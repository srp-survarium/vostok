use std::collections::HashMap;
use std::collections::HashSet;
use std::collections::hash_map;
use std::path::Path;

/// Internal cache with information on already created folders/files.
///
/// This struct is used to build `.vsproject` configuration.
///
/// It also helps to not call `std::fs::create_dir_all` for every single file.
#[derive(Debug, Default)]
pub struct Files {
    pub folders: HashMap<&'static Path, Files>,
    pub files: HashSet<&'static Path>,
}

impl Files {
    /// Returns `true` when a new folder was inserted
    pub fn insert_leak(&mut self, path: &Path) -> bool {
        let path: &'static Path = Box::leak(Box::new(path.to_path_buf()));
        self.insert(path)
    }

    /// Returns `true` when a new folder was inserted
    pub fn insert(&mut self, path: &'static Path) -> bool {
        let mut files = self;

        let mut created_folder = false;

        let mut iter = path.components().peekable();
        while let Some(component) = iter.next() {
            let is_file = iter.peek().is_none();

            match is_file {
                // Note that full path is inserted
                true => _ = files.files.insert(path),

                false => {
                    let folder = Path::new(component.as_os_str());

                    // TODO: Eager allocations suck
                    match files.folders.entry(folder) {
                        hash_map::Entry::Vacant(entry) => {
                            created_folder = true;
                            files = entry.insert(Self::default());
                        }
                        hash_map::Entry::Occupied(entry) => {
                            files = entry.into_mut();
                        }
                    }
                }
            }
        }

        created_folder
    }

    pub fn move_layer_up(&mut self, path: &'static Path) {
        let mut root_files = HashSet::new();
        std::mem::swap(&mut root_files, &mut self.files);

        let root_files = Files {
            folders: HashMap::new(),
            files: root_files,
        };
        self.folders.insert(path, root_files);
    }
}
