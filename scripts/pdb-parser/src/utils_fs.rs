use std::io;

use crate::helpers::Files;

pub fn open_file(
    output_path: &mut std::path::PathBuf,
    source_path: &mut std::path::PathBuf,
    files: &mut Files,
    extension: &'static str,
) -> crate::Result<std::fs::File> {
    let should_create_dir = files.insert_leak(source_path);

    output_path.push(&source_path);

    if should_create_dir {
        std::fs::create_dir_all(output_path.parent().unwrap())?;
    }

    let source_path_len = source_path.as_mut_os_string().len() - extension.len();
    let output_path_len = output_path.as_mut_os_string().len() - extension.len();

    let mut i = 0;
    loop {
        if i != 0 {
            append_to_name(output_path, output_path_len, i, extension);
        }

        let result = std::fs::OpenOptions::new()
            .write(true)
            .create_new(true)
            .open(&output_path);

        match result {
            Err(error) if error.kind() == io::ErrorKind::AlreadyExists => {
                i += 1;
            }

            Ok(file) => {
                if i != 0 {
                    append_to_name(source_path, source_path_len, i, extension);
                }
                files.insert_leak(source_path);

                return Ok(file);
            }

            Err(error) => {
                return Err(error.into());
            }
        }
    }
}

fn append_to_name(path: &mut std::path::PathBuf, len: usize, n: usize, extension: &'static str) {
    use std::fmt::Write;

    let path = path.as_mut_os_string();
    path.truncate(len);

    _ = write!(path, "_{n}{extension}");
}
