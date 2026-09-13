//! Raw Microsoft Multi-Stream File (MSF) container inventory.
//!
//! The high-level `pdb` crate intentionally hides the stream directory and
//! physical page lists.  Those details are useful when auditing whether two
//! PDBs serialize the same logical streams in the same slots and how fragmented
//! each stream is on disk, so this module parses only the small MSF container
//! layer.  CodeView payloads remain owned by the `pdb` crate.

use std::fs::File;
use std::io::{Read, Seek, SeekFrom};
use std::path::Path;

use crate::{Result, bail};

const BIG_MAGIC: &[u8; 32] = b"Microsoft C/C++ MSF 7.00\r\n\x1a\x44\x53\x00\x00\x00";
const SMALL_MAGIC: &[u8; 44] = b"Microsoft C/C++ program database 2.00\r\n\x1a\x4a\x47\x00\x00";

#[derive(Clone, Debug, serde::Serialize)]
pub struct MsfLayout {
    pub format: &'static str,
    pub page_size: u32,
    pub free_page_map: u32,
    pub pages_used: u32,
    pub directory_size: u32,
    pub free_page_map_pages: Vec<u32>,
    pub free_pages: Vec<u32>,
    pub directory_map_pages: Vec<u32>,
    pub directory_pages: Vec<u32>,
    pub streams: Vec<MsfStreamLayout>,
}

#[derive(Clone, Debug, serde::Serialize)]
pub struct MsfStreamLayout {
    pub index: u32,
    /// `None` is the directory's `0xffff_ffff` absent-stream sentinel.  An
    /// empty but present stream is represented as `Some(0)`.
    pub size: Option<u32>,
    pub pages: Vec<u32>,
}

impl MsfLayout {
    pub fn parse(path: &Path) -> Result<Self> {
        let mut file = File::open(path)?;
        let mut prefix = [0u8; 64];
        file.read_exact(&mut prefix)?;
        let page_size = if prefix.starts_with(BIG_MAGIC) {
            le_u32(&prefix, 32)?
        } else if prefix.starts_with(SMALL_MAGIC) {
            le_u32(&prefix, 44)?
        } else {
            bail!("unrecognized MSF header in {}", path.display())
        };
        validate_page_size(page_size)?;
        let mut header = vec![0u8; page_size as usize];
        file.seek(SeekFrom::Start(0))?;
        file.read_exact(&mut header)?;

        if header.starts_with(BIG_MAGIC) {
            Self::parse_big(file, &header)
        } else {
            Self::parse_small(file, &header)
        }
    }

    pub fn stream(&self, index: u32) -> Option<&MsfStreamLayout> {
        self.streams.get(index as usize)
    }

    pub fn read_stream(&self, path: &Path, index: u32) -> Result<Option<Vec<u8>>> {
        let Some(stream) = self.stream(index) else {
            return Ok(None);
        };
        let Some(size) = stream.size else {
            return Ok(None);
        };
        let mut file = File::open(path)?;
        let mut bytes = read_pages(&mut file, self.page_size, self.pages_used, &stream.pages)?;
        bytes.truncate(size as usize);
        Ok(Some(bytes))
    }

    fn parse_big(mut file: File, header: &[u8]) -> Result<Self> {
        let page_size = le_u32(header, 32)?;
        validate_page_size(page_size)?;
        let free_page_map = le_u32(header, 36)?;
        let pages_used = le_u32(header, 40)?;
        let directory_size = le_u32(header, 44)?;
        validate_file_size(&file, page_size, pages_used)?;

        let directory_page_count = pages_for(directory_size, page_size);
        let map_page_count = pages_for(directory_page_count.saturating_mul(4), page_size);
        let mut directory_map_pages = Vec::with_capacity(map_page_count as usize);
        let mut cursor = 52usize;
        for _ in 0..map_page_count {
            let page = le_u32(header, cursor)?;
            validate_page(page, pages_used)?;
            directory_map_pages.push(page);
            cursor += 4;
        }

        let map = read_pages(&mut file, page_size, pages_used, &directory_map_pages)?;
        let mut directory_pages = Vec::with_capacity(directory_page_count as usize);
        for position in 0..directory_page_count as usize {
            let page = le_u32(&map, position * 4)?;
            validate_page(page, pages_used)?;
            directory_pages.push(page);
        }
        let mut directory = read_pages(&mut file, page_size, pages_used, &directory_pages)?;
        directory.truncate(directory_size as usize);
        let streams = parse_big_directory(&directory, page_size, pages_used)?;
        let (free_page_map_pages, free_pages) =
            load_free_page_map(&mut file, page_size, pages_used, free_page_map)?;

        Ok(Self {
            format: "MSF 7.00",
            page_size,
            free_page_map,
            pages_used,
            directory_size,
            free_page_map_pages,
            free_pages,
            directory_map_pages,
            directory_pages,
            streams,
        })
    }

    fn parse_small(mut file: File, header: &[u8]) -> Result<Self> {
        let page_size = le_u32(header, 44)?;
        validate_page_size(page_size)?;
        let free_page_map = le_u16(header, 48)? as u32;
        let pages_used = le_u16(header, 50)? as u32;
        let directory_size = le_u32(header, 52)?;
        validate_file_size(&file, page_size, pages_used)?;

        let directory_page_count = pages_for(directory_size, page_size);
        let mut directory_pages = Vec::with_capacity(directory_page_count as usize);
        let mut cursor = 60usize;
        for _ in 0..directory_page_count {
            let page = le_u16(header, cursor)? as u32;
            validate_page(page, pages_used)?;
            directory_pages.push(page);
            cursor += 2;
        }
        let mut directory = read_pages(&mut file, page_size, pages_used, &directory_pages)?;
        directory.truncate(directory_size as usize);
        let streams = parse_small_directory(&directory, page_size, pages_used)?;
        let (free_page_map_pages, free_pages) =
            load_free_page_map(&mut file, page_size, pages_used, free_page_map)?;

        Ok(Self {
            format: "MSF 2.00",
            page_size,
            free_page_map,
            pages_used,
            directory_size,
            free_page_map_pages,
            free_pages,
            directory_map_pages: Vec::new(),
            directory_pages,
            streams,
        })
    }
}

impl MsfStreamLayout {
    #[must_use]
    pub fn page_runs(&self) -> usize {
        if self.pages.is_empty() {
            return 0;
        }
        1 + self
            .pages
            .windows(2)
            .filter(|pair| pair[1] != pair[0].saturating_add(1))
            .count()
    }
}

fn parse_big_directory(
    directory: &[u8],
    page_size: u32,
    pages_used: u32,
) -> Result<Vec<MsfStreamLayout>> {
    let stream_count = le_u32(directory, 0)?;
    let mut cursor = 4usize;
    let mut sizes = Vec::with_capacity(stream_count as usize);
    for _ in 0..stream_count {
        let size = le_u32(directory, cursor)?;
        cursor += 4;
        sizes.push((size != u32::MAX).then_some(size));
    }
    parse_stream_pages(directory, cursor, sizes, page_size, pages_used, 4)
}

fn parse_small_directory(
    directory: &[u8],
    page_size: u32,
    pages_used: u32,
) -> Result<Vec<MsfStreamLayout>> {
    let stream_count = le_u16(directory, 0)? as u32;
    let mut cursor = 4usize; // count plus reserved u16
    let mut sizes = Vec::with_capacity(stream_count as usize);
    for _ in 0..stream_count {
        let size = le_u32(directory, cursor)?;
        cursor += 8; // size plus reserved u32
        sizes.push((size != u32::MAX).then_some(size));
    }
    parse_stream_pages(directory, cursor, sizes, page_size, pages_used, 2)
}

fn parse_stream_pages(
    directory: &[u8],
    mut cursor: usize,
    sizes: Vec<Option<u32>>,
    page_size: u32,
    pages_used: u32,
    page_number_size: usize,
) -> Result<Vec<MsfStreamLayout>> {
    let mut streams = Vec::with_capacity(sizes.len());
    for (index, size) in sizes.into_iter().enumerate() {
        let mut pages = Vec::new();
        if let Some(size) = size {
            pages.reserve(pages_for(size, page_size) as usize);
            for _ in 0..pages_for(size, page_size) {
                let page = if page_number_size == 4 {
                    le_u32(directory, cursor)?
                } else {
                    le_u16(directory, cursor)? as u32
                };
                cursor += page_number_size;
                validate_page(page, pages_used)?;
                pages.push(page);
            }
        }
        streams.push(MsfStreamLayout {
            index: index as u32,
            size,
            pages,
        });
    }
    Ok(streams)
}

fn read_pages(file: &mut File, page_size: u32, pages_used: u32, pages: &[u32]) -> Result<Vec<u8>> {
    let mut output = vec![0u8; pages.len().saturating_mul(page_size as usize)];
    for (position, &page) in pages.iter().enumerate() {
        validate_page(page, pages_used)?;
        file.seek(SeekFrom::Start(u64::from(page) * u64::from(page_size)))?;
        let start = position * page_size as usize;
        file.read_exact(&mut output[start..start + page_size as usize])?;
    }
    Ok(output)
}

fn load_free_page_map(
    file: &mut File,
    page_size: u32,
    pages_used: u32,
    first_page: u32,
) -> Result<(Vec<u32>, Vec<u32>)> {
    if first_page != 1 && first_page != 2 {
        bail!("MSF active free-page map begins at page {first_page}, expected 1 or 2")
    }
    let map_page_count = pages_used.div_ceil(page_size.saturating_mul(8));
    let mut map_pages = Vec::with_capacity(map_page_count as usize);
    for position in 0..map_page_count {
        let page = first_page
            .checked_add(position.saturating_mul(page_size))
            .ok_or_else(|| crate::Error::new("MSF free-page map index overflow".into()))?;
        validate_page(page, pages_used)?;
        map_pages.push(page);
    }
    let bits = read_pages(file, page_size, pages_used, &map_pages)?;
    let mut free_pages = Vec::new();
    for page in 0..pages_used {
        if bits[page as usize / 8] & (1 << (page % 8)) != 0 {
            free_pages.push(page);
        }
    }
    Ok((map_pages, free_pages))
}

fn validate_file_size(file: &File, page_size: u32, pages_used: u32) -> Result<()> {
    let required = u64::from(page_size) * u64::from(pages_used);
    let actual = file.metadata()?.len();
    if actual < required {
        bail!("MSF declares {required} bytes but file has {actual}")
    }
    Ok(())
}

fn validate_page_size(page_size: u32) -> Result<()> {
    if !page_size.is_power_of_two() || !(0x100..=128 * 0x10000).contains(&page_size) {
        bail!("invalid MSF page size 0x{page_size:x}")
    }
    Ok(())
}

fn validate_page(page: u32, pages_used: u32) -> Result<()> {
    if page == 0 || page >= pages_used {
        bail!("MSF page {page} is outside 1..{pages_used}")
    }
    Ok(())
}

fn pages_for(bytes: u32, page_size: u32) -> u32 {
    bytes.div_ceil(page_size)
}

fn le_u16(bytes: &[u8], offset: usize) -> Result<u16> {
    let Some(value) = bytes.get(offset..offset.saturating_add(2)) else {
        bail!("short MSF read at 0x{offset:x}")
    };
    Ok(u16::from_le_bytes([value[0], value[1]]))
}

fn le_u32(bytes: &[u8], offset: usize) -> Result<u32> {
    let Some(value) = bytes.get(offset..offset.saturating_add(4)) else {
        bail!("short MSF read at 0x{offset:x}")
    };
    Ok(u32::from_le_bytes([value[0], value[1], value[2], value[3]]))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn put_u32(bytes: &mut [u8], offset: usize, value: u32) {
        bytes[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
    }

    #[test]
    fn parses_big_msf_stream_slots_and_physical_pages() {
        let page_size = 0x100usize;
        let mut bytes = vec![0u8; page_size * 10];
        bytes[..BIG_MAGIC.len()].copy_from_slice(BIG_MAGIC);
        put_u32(&mut bytes, 32, page_size as u32);
        put_u32(&mut bytes, 36, 1);
        put_u32(&mut bytes, 40, 10);
        put_u32(&mut bytes, 44, 28);
        put_u32(&mut bytes, 52, 3); // directory-map page
        put_u32(&mut bytes, page_size * 3, 4); // directory page

        let directory = page_size * 4;
        put_u32(&mut bytes, directory, 3);
        put_u32(&mut bytes, directory + 4, 0);
        put_u32(&mut bytes, directory + 8, 4);
        put_u32(&mut bytes, directory + 12, 300);
        put_u32(&mut bytes, directory + 16, 5);
        put_u32(&mut bytes, directory + 20, 6);
        put_u32(&mut bytes, directory + 24, 7);
        bytes[page_size * 5..page_size * 5 + 4].copy_from_slice(b"info");
        bytes[page_size * 6..page_size * 6 + 256].fill(0xa5);
        bytes[page_size * 7..page_size * 7 + 44].fill(0x5a);
        bytes[page_size + 1] = 0b0000_0011; // pages 8 and 9 are free

        let path = std::env::temp_dir().join(format!(
            "vostok-pdb-parser-msf-layout-{}.pdb",
            uuid::Uuid::new_v4()
        ));
        std::fs::write(&path, bytes).unwrap();
        let layout = MsfLayout::parse(&path).unwrap();
        assert_eq!(layout.format, "MSF 7.00");
        assert_eq!(layout.free_page_map_pages, [1]);
        assert_eq!(layout.free_pages, [8, 9]);
        assert_eq!(layout.directory_map_pages, [3]);
        assert_eq!(layout.directory_pages, [4]);
        assert_eq!(layout.streams.len(), 3);
        assert_eq!(layout.streams[0].size, Some(0));
        assert!(layout.streams[0].pages.is_empty());
        assert_eq!(layout.streams[1].pages, [5]);
        assert_eq!(layout.streams[2].pages, [6, 7]);
        assert_eq!(layout.streams[2].page_runs(), 1);
        let stream = layout.read_stream(&path, 2).unwrap().unwrap();
        assert_eq!(stream.len(), 300);
        assert!(stream[..256].iter().all(|byte| *byte == 0xa5));
        assert!(stream[256..].iter().all(|byte| *byte == 0x5a));
        std::fs::remove_file(path).unwrap();
    }
}
