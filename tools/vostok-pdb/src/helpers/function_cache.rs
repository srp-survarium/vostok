use std::collections::HashMap;

use pdb::ItemIndex;
use pdb_addr2line::type_parser;

use crate::Type;
use crate::gen_sources;
use crate::pdb_parser;

/// Iterating through modules gives me names of the arguments.
/// But the class name IS NOT removed from the function name,
/// resulting in signatures like so:
/// `void survarium::bullet_manager::tick()`.
///
/// While when iterating through classes, argument names are not provided.
/// But the class name IS removed from the function name,
/// resulting in signatures like so:
/// `void tick()`.
///
/// So to properly find arguments in the cache (to generate headers with them).
/// I convert the function name from sources files to:
/// `void bullet_manager::tick()` (by removing `survarium::`)
///
/// While on the header size the class name is appended:
/// `void bullet_manager::tick()`
///
/// This allows me to match on a cache signatures and provide arguments in the header.
pub struct FunctionCache {
    // Original Name -> FunctionSignature
    pub cache: HashMap<String, FunctionSignature>,
}

#[derive(Debug, Clone)]
pub struct FunctionSignature {
    pub fn_t: type_parser::Function,
    pub margs: Vec<(String, Type)>,
}

impl FunctionCache {
    pub fn new() -> Self {
        Self {
            cache: Default::default(),
        }
    }

    pub fn insert_from_source(&mut self, fun: &gen_sources::Function) {
        let gen_sources::Function {
            name_orig,
            fn_t,
            margs,
            ..
        } = fun.clone();

        let margs = margs
            .into_iter()
            .map(|(t, n)| (t.to_string().to_string(), n))
            .collect::<Vec<_>>();

        self.cache
            .insert(name_orig, FunctionSignature { fn_t, margs });
    }

    pub fn get_from_header(
        &self,
        class_name: &str,
        name: &pdb::RawString,
        formatter: &pdb_parser::PdbParser,
        type_index: pdb::TypeIndex,
    ) -> crate::Result<Option<FunctionSignature>> {
        assert!(!type_index.is_cross_module());

        let cache_method_name = {
            let name = format!("{class_name}::{}", name.to_string());
            let name = pdb::RawString::from(name.as_bytes());

            formatter.emit_function_orig(&name, 0, type_index)?
        };

        let mut signature = self.cache.get(&cache_method_name).cloned();
        if let Some(signature) = &mut signature {
            signature.fn_t.name = signature
                .fn_t
                .name
                .strip_prefix(class_name)
                .unwrap()
                .strip_prefix("::")
                .unwrap()
                .to_string();
        }

        Ok(signature)
    }
}
