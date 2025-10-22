#![feature(str_as_str)]
#![feature(os_string_truncate)]
#![expect(clippy::len_without_is_empty)]

//! Builds a project structure out of the provided PDB file.
//!
//! Hardcoded to work only with `survarium.pdb`, though this can be improved later on.
//! (See constants in `run` module)
//!
//! Execute from the root of the workspace like so:
//!
//! ```ignore
//! cargo run --bin pdb-parser --release -- --pdb_path="D:/Projects/Survarium/binaries/win32/survarium.pdb" --output_path="../vostok-structure"
//! ```
//!
//! The values are hardocded for ease of use by me, so if your paths are the same as in the example
//! above, you can simply run:
//!
//! ```ignore
//! cargo run --bin pdb-parser --release
//! ```

// @TODO: Deal with naemspaces. Do not hardcode them.
// @TODO: Add SAFE flag for deleting `source` and `header` folders

pub mod data;
pub mod pdb_parser;

pub mod error;
pub mod utils;
pub mod utils_fs;

pub mod dump_pdb;
pub mod gen_headers;
pub mod gen_sources;

pub use error::{Error, Result};
pub use utils::Type;

use clap::Parser;

#[derive(clap::Parser)]
pub struct Cli {
    #[arg(
        short,
        long,
        value_hint = clap::ValueHint::FilePath,
    )]
    pdb_path: std::path::PathBuf,

    #[arg(
        short,
        long,
        value_hint = clap::ValueHint::FilePath,
    )]
    output_path: std::path::PathBuf,

    #[arg(
        short,
        long,
        value_hint = clap::ValueHint::FilePath,
    )]
    engine_path: String,

    // cargo run --bin pdb-parser --release -- --test-run > ./target/survarium.txt ; if ($?) { nvim ./target/survarium.txt }
    #[arg(long, action)]
    test_run: bool,

    #[arg(long, action)]
    as_base: bool,

    #[arg(long, action)]
    no_cache: bool,

    #[arg(long, action)]
    no_overwrites: bool,

    #[arg(long, action)]
    skip_non_engine_headers: bool,
}

bitflags::bitflags! {
    #[derive(Default, Copy, Clone)]
    pub struct GenFlags: u32 {
        /// Do not generate file structure.
        /// Print to `stdout` source file for `TEST_MODULE` instead.
        const TEST_RUN                = 0b0000_0001;

        /// Generating for `BASE`.
        /// i.e. the stub is generated for the `xray` code being modified
        /// as opposed to `TARGET`, to which the code is being matched.
        ///
        /// This will cause comments to be slightly different with another prefix used for files.
        const AS_BASE                 = 0b0000_0010;

        /// Do not use cache with names for generating member function declarations in headers.
        /// This is useful right now, since there are conflicts because of namespaces:
        /// `network_core::http_client::update` will conflict with `network::http_client::update`.
        const NO_CACHE                = 0b0000_0100;

        /// Do not overwrite header files, instead append N to their names.
        /// While this is not useful in general, I've seen that there are legit overwrites and this
        /// needs to be investigated.
        const NO_OVERWRITES           = 0b0000_1000;

        ///
        const SKIP_NON_ENGINE_HEADERS = 0b0001_0000;
    }
}

pub const TEST_MODULE: &str = "bullet_manager.obj";

fn main() {
    let Cli {
        pdb_path,
        output_path,
        engine_path,
        test_run,
        as_base,
        no_cache,
        no_overwrites,
        skip_non_engine_headers,
    } = Cli::parse();

    let flags = {
        let mut flags = GenFlags::empty();
        flags.set(GenFlags::TEST_RUN, test_run);
        flags.set(GenFlags::AS_BASE, as_base);
        flags.set(GenFlags::NO_CACHE, no_cache);
        flags.set(GenFlags::NO_OVERWRITES, no_overwrites);
        flags.set(GenFlags::SKIP_NON_ENGINE_HEADERS, skip_non_engine_headers);
        flags
    };

    let mut engine_path = engine_path.to_lowercase().replace('/', "\\");
    if !engine_path.ends_with('\\') {
        engine_path.push('\\');
    }

    if let Err(error) = dump_pdb::dump_pdb(&pdb_path, &output_path, &engine_path, flags) {
        eprintln!("{error}");
        std::process::exit(1);
    }
}
