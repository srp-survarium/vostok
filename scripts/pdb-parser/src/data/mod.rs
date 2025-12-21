mod files;
mod function_cache;

pub use files::Files;
pub use function_cache::{FunctionCache, FunctionSignature};

#[derive(Debug, Clone)]
pub enum FunctionLocation {
    Header,
    Source,
}

impl FunctionLocation {
    pub fn get(file: &str) -> Self {
        match std::path::Path::new(file).extension() {
            None => Self::Header,
            Some(ext) => match ext.as_encoded_bytes() {
                b"cpp" | b"c" | b"cxx" => Self::Source,
                _ => Self::Header,
            },
        }
    }
}
