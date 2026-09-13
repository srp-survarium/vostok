#![feature(str_as_str)]
#![feature(os_string_truncate)]
#![feature(trim_prefix_suffix)]
#![expect(clippy::len_without_is_empty)]

pub mod divergence;
pub mod dump_pdb;
pub mod evidence;
pub mod gen_headers;
pub mod gen_sources;

pub mod disasm;
pub mod rich_context;
pub mod rich_diff;
pub mod rich_objdiff;
pub mod rich_render;
pub mod rich_structure_diff;
pub mod source;
pub mod topology;

pub mod formatter;
pub mod helpers;
pub mod msf_layout;
pub mod pdb_parser;
pub mod type_builder;

pub mod error;
pub mod utils_fs;

pub use error::{Error, Result};
pub use type_builder::{Namespace, Type};

#[derive(clap::Parser)]
pub struct Cli {
    #[arg(
        short,
        long,
        value_hint = clap::ValueHint::FilePath,
    )]
    pub pdb_path: std::path::PathBuf,

    #[arg(
        short,
        long,
        value_hint = clap::ValueHint::FilePath,
    )]
    pub output_path: std::path::PathBuf,

    #[arg(
        short,
        long,
        value_hint = clap::ValueHint::FilePath,
    )]
    pub engine_path: String,

    #[arg(long, action)]
    pub as_base: bool,

    #[arg(long, action)]
    pub no_cache: bool,

    #[arg(long, action)]
    pub skip_non_engine_headers: bool,
}

bitflags::bitflags! {
    #[derive(Default, Copy, Clone)]
    pub struct GenFlags: u32 {
        /// Generating for `BASE`.
        const AS_BASE                 = 0b0000_0010;

        /// Do not use cache with names for generating member function declarations in headers.
        const NO_CACHE                = 0b0000_0100;

        /// Simple optimization to skip generating headers for classes not in `vostok` and
        /// `survarium` namespaces.
        const SKIP_NON_ENGINE_HEADERS = 0b0001_0000;
    }
}
