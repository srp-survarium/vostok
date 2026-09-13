use pdb_addr2line::type_parser;
use pdb_addr2line::type_parser::AttributeFlags;

use pdb_addr2line::{ContextPdbData as Data, TypeFormatterFlags as Flags, pdb::PDB as Pdb};
use pdb_addr2line_orig::{
    ContextPdbData as DataOrig, TypeFormatterFlags as FlagsOrig, pdb::PDB as PdbOrig,
};

use crate::{Namespace, Type};

/// Formatter for types and functions.
// A hacky way to store different types of formatters.
// This is bad, since this will keep 2 versions of the `pdb` file in memory.
// The proper solution would update `pdb_addr2line` crate to allow passing flags into functions
pub struct PdbParser<'a, 's> {
    /// Updated formatter specifically for xray project.
    /// The differences are:
    /// * Prints `s8`, `s32`, `i8`, `i32` types instead of standard primitive types.
    /// * Skips `const` in function arguments for values.
    formatter: pdb_addr2line::TypeFormatter<'a, 's>,

    formatter_orig: pdb_addr2line_orig::TypeFormatter<'a, 's>,
}

impl<'a, 's> PdbParser<'a, 's> {
    /// Run a closure with the formatter initialized.
    pub fn with<T>(
        filename: &std::path::Path,
        format: impl for<'aa, 'ss> FnOnce(PdbParser<'aa, 'ss>) -> crate::Result<T>,
    ) -> crate::Result<T> {
        let file = std::fs::File::open(filename)?;
        {
            let pdb = Pdb::open(&file)?;
            let data = Data::try_from_pdb(pdb)?;
            let formatter = data.make_type_formatter_with_flags(
                Flags::SPACE_AFTER_COMMA | Flags::NAME_ONLY | Flags::NO_ARGUMENTS,
            )?;

            let pdb_orig = PdbOrig::open(&file)?;
            let data_orig = DataOrig::try_from_pdb(pdb_orig)?;
            let formatter_orig = data_orig.make_type_formatter_with_flags(
                FlagsOrig::SPACE_AFTER_COMMA | FlagsOrig::NAME_ONLY,
            )?;

            return format(PdbParser {
                formatter,
                formatter_orig,
            });
        }
    }

    pub fn parse_function(
        &self,
        proc_name: &pdb::RawString,
        module_id: usize,
        type_index: pdb::TypeIndex,
    ) -> pdb_addr2line::Result<pdb_addr2line::type_parser::Function> {
        self.formatter
            .parse_function(&proc_name.to_string(), module_id, ti(type_index))
    }

    // @TODO: Get rid of module_id | add assert too
    pub fn emit_type(
        &self,
        module_id: usize,
        type_index: pdb::TypeIndex,
        namespace: &Namespace,
    ) -> crate::Result<Type> {
        self.emit_type_impl(module_id, type_index)
            .map(|ty| Type::new(&ty, namespace))
    }

    pub fn emit_type_impl(
        &self,
        module_id: usize,
        type_index: pdb::TypeIndex,
    ) -> crate::Result<String> {
        use pdb_addr2line::pdb::TypeData;

        let mut type_name = String::new();
        self.formatter.for_module(module_id, |tf| {
            let index = ti(type_index);
            // The formatter renders only the `constant` bit of LF_MODIFIER and
            // drops `volatile`, so volatile-qualified types (e.g. the
            // interlocked-guarded `volatile long` reference counters) are
            // rendered here instead. const-only modifiers keep the formatter's
            // own path so their placement rules stay untouched.
            match tf.parse_type_index(index) {
                Ok(TypeData::Modifier(modifier)) if modifier.volatile => {
                    match tf.parse_type_index(modifier.underlying_type) {
                        // LF_MODIFIER wrapping a pointer qualifies the pointer
                        // itself: `T* volatile` / `T* const volatile`.
                        Ok(TypeData::Pointer(pointer)) => {
                            // the bumped fork renders `* volatile` itself via the 4th arg
                            tf.emit_ptr(&mut type_name, pointer, modifier.constant, true)?;
                            Ok(())
                        }
                        Ok(underlying) => {
                            if modifier.constant {
                                type_name.push_str("const ");
                            }
                            type_name.push_str("volatile ");
                            tf.emit_type(&mut type_name, underlying)
                        }
                        Err(error) => Err(error),
                    }
                }
                // `T* volatile` can also be recorded directly in the pointer
                // record's attribute bits, without an LF_MODIFIER wrapper
                // (e.g. engine_world::m_sound_world). The formatter renders
                // the const attribute bit but drops the volatile one.
                Ok(TypeData::Pointer(pointer)) if pointer.attributes.is_volatile() => {
                    tf.emit_ptr(&mut type_name, pointer, false, true)?;
                    Ok(())
                }
                _ => tf.emit_type_index(&mut type_name, index),
            }
        })?;
        Ok(type_name)
    }

    pub fn emit_function_orig(
        &self,
        proc_name: &pdb::RawString,
        module_id: usize,
        type_index: pdb::TypeIndex,
    ) -> crate::Result<String> {
        let mut name = String::new();
        self.formatter_orig.emit_function(
            &mut name,
            proc_name.to_string().as_str(),
            module_id,
            ti(type_index),
        )?;
        Ok(name)
    }
}

fn ti(type_index: pdb::TypeIndex) -> pdb_addr2line::pdb::TypeIndex {
    pdb_addr2line::pdb::TypeIndex(type_index.0)
}

//
//
//

pub fn set_method_attributes(
    fn_t: &mut type_parser::Function,
    attrs: pdb::FieldAttributes,
    _found_body: bool,
) {
    let attrs = method_attributes::MyFieldAttributes::extract(attrs);

    #[rustfmt::skip]
    {
        fn_t.attrs.set(AttributeFlags::IS_VIRTUAL,  attrs.is_virtual());
        fn_t.attrs.set(AttributeFlags::IS_OVERRIDE, attrs.is_override());
        fn_t.attrs.set(AttributeFlags::IS_PURE,     attrs.is_pure());
        fn_t.attrs.set(AttributeFlags::IS_FINAL,    attrs.is_final());

        // An absent procedure can also mean unused, optimized away, folded, or
        // emitted only by a different consumer. It is not source-inline proof.
        fn_t.attrs.set(
            AttributeFlags::IS_INLINE,
            fn_t.attrs.contains(AttributeFlags::IS_INLINE),
        );

    };
}

mod method_attributes {
    #[derive(Debug, Copy, Clone, PartialEq, Eq)]
    pub struct MyFieldAttributes(pub u16);

    impl MyFieldAttributes {
        pub fn extract(val: pdb::FieldAttributes) -> Self {
            let s = format!("{val:?}",);
            let s = s
                .trim_start_matches("FieldAttributes(")
                .trim_end_matches(')')
                .parse::<u16>()
                .unwrap();
            Self(s)
        }

        #[inline]
        #[must_use]
        fn method_properties(self) -> u8 {
            ((self.0 & 0x001c) >> 2) as u8
        }

        #[inline]
        #[must_use]
        pub fn is_pure(self) -> bool {
            matches!(self.method_properties(), 0x05 | 0x06)
        }

        #[inline]
        #[must_use]
        pub fn is_virtual(self) -> bool {
            matches!(self.method_properties(), 0x01 | 0x04 | 0x05 | 0x06)
        }

        #[inline]
        pub fn is_override(self) -> bool {
            matches!(self.method_properties(), 0x01 | 0x05)
        }

        #[inline]
        #[must_use]
        pub fn is_final(self) -> bool {
            self.0 & 0x0200 != 0
        }

        /*

        #[inline]
        #[must_use]
        pub fn noinherit(self) -> bool {
            self.0 & 0x0040 != 0
        }

        #[inline]
        #[must_use]
        pub fn noconstruct(self) -> bool {
            self.0 & 0x0080 != 0
        }

        */
    }
}
