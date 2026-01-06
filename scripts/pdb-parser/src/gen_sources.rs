use std::collections::BTreeMap;
use std::collections::BTreeSet;
use std::collections::HashSet;
use std::path::Path;
use std::sync::LazyLock;

use pdb::ConstantSymbol;
use pdb::DataSymbol;
use pdb::RegisterRelativeSymbol;
use pdb::RegisterVariableSymbol;
use pdb::{BasePointerRelativeSymbol, BlockSymbol, FallibleIterator, SymbolData};
use pdb_addr2line::type_parser;
use pdb_addr2line::type_parser::AttributeFlags;

use crate::helpers::FunctionLocation;
use crate::helpers::{Files, FunctionCache};
use crate::pdb_parser::PdbParser;
use crate::GenFlags;
use crate::{Namespace, Type};

use crate::formatter;
use crate::utils_fs;

const GAME_IB: u32 = 0x10000;

#[derive(Clone)]
pub struct Function<'a> {
    pub module_id: usize,
    pub type_index: pdb::TypeIndex,
    pub flags: GenFlags,

    pub fn_t: pdb_addr2line::type_parser::Function,
    pub offset: pdb::Rva,
    pub name_orig: String,
    pub namespace: Namespace,

    pub margs: Vec<(pdb::RawString<'a>, Type)>,
    pub locals: Vec<(pdb::RawString<'a>, Type, usize)>,

    pub proc_start: u32,
    pub proc_end: u32,
    pub statements: Vec<Statement>,

    pub constants: Vec<(pdb::RawString<'a>, Type, pdb::Variant)>,
    pub statics: Vec<(pdb::RawString<'a>, Type, pdb::Rva)>,

    pub blocks: Vec<(pdb::Rva, i32)>,
    pub typedefs: Vec<(Type, Type)>,
    pub callsites: Vec<(pdb::Rva, Type)>,
    pub symbols: Vec<pdb::SymbolData<'a>>,
}

#[derive(Default, Clone)]
pub struct Statement {
    rva: pdb::Rva,
    line_start: u32,
    depth: i32,
}

//
//
//

pub fn dump_sources(
    pdb: &mut pdb::PDB<std::fs::File>,
    formatter: &PdbParser,
    output_path: &std::path::Path,
    engine_path: &str,
    flags: GenFlags,
    files: &mut Files,
) -> crate::Result<FunctionCache> {
    let address_map = pdb.address_map()?;
    let string_table = pdb.string_table()?;

    let mut output_path = output_path.to_path_buf();
    let mut source_path = Path::new("sources").to_path_buf();

    let dbi = pdb.debug_information()?;

    let mut modules = dbi.modules()?;
    let mut module_id: usize = usize::MAX;

    let mut cache = FunctionCache::new();

    while let Some(module) = modules.next()? {
        module_id = module_id.wrapping_add(1);

        if module.module_name().contains("\\scaleform\\") {
            continue;
        }

        // private symbols
        let Some(module_info) = pdb.module_info(&module)? else {
            continue;
        };

        let module = Module::build(
            &module_info,
            module_id,
            formatter,
            &address_map,
            &string_table,
            flags,
        )?;

        module.update_cache(&mut cache, flags);
        module.write(&mut output_path, &mut source_path, engine_path, files)?;
    }

    Ok(cache)
}

struct Module<'a> {
    files: BTreeMap<String, BTreeMap<u32, Function<'a>>>,
    // typedef void* ptr;
    //         ^     ^
    //         type  name
    typedefs: BTreeSet<(Type, Type)>,
    //                  type  name
}

impl<'a> Module<'a> {
    // Returns a mapping for a module: filename -> proc_start -> Function
    fn build(
        module_info: &'a pdb::ModuleInfo,
        module_id: usize,

        formatter: &PdbParser,
        address_map: &pdb::AddressMap,
        string_table: &pdb::StringTable,

        flags: GenFlags,
    ) -> crate::Result<Self> {
        let program = module_info.line_program()?;

        let mut symbols = module_info.symbols()?;

        let mut files: BTreeMap<String, BTreeMap<u32, Function>> = BTreeMap::new();
        let mut typedefs: BTreeSet<(Type, Type)> = BTreeSet::new();

        let mut function: Function = Function::new(flags);
        let mut depth: i32 = 0;

        let mut filename: String = String::new();

        while let Some(symbol) = symbols.next()? {
            match symbol.parse()? {
                // FunctionStart
                SymbolData::Procedure(proc) => {
                    assert_eq!(
                        depth, 0,
                        "Function cannot be defined inside another function"
                    );
                    depth += 1;

                    let mut m_proc_start = None;
                    let mut m_proc_end = None;

                    let mut m_file_name = None;

                    let mut breakpoints = Vec::new();

                    //
                    //
                    //

                    let mut lines = program.lines_for_symbol(proc.offset);
                    while let Some(line_info) = lines.next()? {
                        if m_proc_start.is_none() {
                            m_proc_start = Some(line_info.line_start);
                        }
                        m_proc_end = Some(line_info.line_start);

                        let file_name = {
                            let file_info = program.get_file_info(line_info.file_index)?;
                            file_info.name.to_string_lossy(string_table)?
                        };
                        match &m_file_name {
                            None => m_file_name = Some(file_name),
                            Some(m_file_name) => assert_eq!(*m_file_name, file_name),
                        }

                        let rva = line_info.offset.to_rva(address_map).expect("invalid rva");
                        breakpoints.push(Statement {
                            rva,
                            line_start: line_info.line_start,
                            depth: 0,
                        });
                    }

                    //
                    //
                    //

                    let Some(proc_start) = m_proc_start else {
                        continue;
                    };

                    let Some(proc_end) = m_proc_end else {
                        continue;
                    };

                    let Some(file_name) = m_file_name else {
                        continue;
                    };

                    filename = file_name.to_string();
                    // TODO: Skip creating function here. We already have a filename to know
                    // whether it will be written to `structure` or not.

                    let offset = proc.offset.to_rva(address_map).expect("invalid rva");
                    let name_orig =
                        formatter.emit_function_orig(&proc.name, module_id, proc.type_index)?;

                    let mut fn_t =
                        formatter.parse_function(&proc.name, module_id, proc.type_index)?;

                    let location = FunctionLocation::get(&filename);
                    if matches!(location, FunctionLocation::Header) {
                        fn_t.attrs.insert(AttributeFlags::IS_INLINE);
                    }

                    function = Function {
                        module_id,
                        type_index: proc.type_index,
                        flags: function.flags,
                        //
                        name_orig,
                        namespace: Namespace::get_from_class_name(&fn_t),
                        //
                        fn_t,
                        offset,
                        //
                        proc_start,
                        proc_end,
                        statements: breakpoints,
                        //
                        margs: Default::default(),
                        locals: Default::default(),
                        //
                        constants: Default::default(),
                        statics: Default::default(),
                        //
                        blocks: Default::default(),
                        typedefs: Default::default(),
                        callsites: Default::default(),
                        symbols: Default::default(),
                    };
                }

                // FunctionEnd
                SymbolData::ScopeEnd if depth == 1 => {
                    let mut take_filename = String::new();
                    std::mem::swap(&mut take_filename, &mut filename);

                    let mut take_function = Function::new(function.flags);
                    std::mem::swap(&mut take_function, &mut function);

                    let args_count = take_function.fn_t.arg_types.len();

                    let locals = take_function
                        .margs
                        .split_off(args_count.min(take_function.margs.len()))
                        .into_iter()
                        .map(|(local_name, local_type)| (local_name, local_type, 0));
                    // This deals with there being too many args
                    take_function.locals.extend(locals);

                    files
                        .entry(take_filename)
                        .or_default()
                        .insert(take_function.proc_start, take_function);

                    depth -= 1;
                }

                // Arguments & Locals
                //
                // @NOTE: This is an approximation and will sometimes be incorrect.
                // There is no simple way to get actual types from `pdb` thanks to LTCG
                // and other optimizations.
                SymbolData::BasePointerRelative(BasePointerRelativeSymbol {
                    offset,
                    type_index,
                    name,
                    slot: _,
                }) if depth >= 1 => {
                    let local_name = name;
                    let local_type =
                        formatter.emit_type(module_id, type_index, &function.namespace)?;

                    if function.locals.is_empty() && local_name.as_bytes() == b"this" {
                        // Skip `this` since it is an implicit argument.
                    } else if offset >= 0 {
                        function.margs.push((local_name, local_type));
                    } else {
                        function
                            .locals
                            .push((local_name, local_type, depth as usize - 1));
                    }
                }

                // Arguments & Locals
                //
                // @NOTE: This is an approximation and will sometimes be incorrect.
                // There is no simple way to get actual types from `pdb` thanks to LTCG
                // and other optimizations.
                SymbolData::RegisterRelative(RegisterRelativeSymbol {
                    offset: _,
                    type_index,
                    register: _,
                    name,
                    slot: _,
                })
                | SymbolData::RegisterVariable(RegisterVariableSymbol {
                    type_index,
                    register: _,
                    name,
                    slot: _,
                }) if depth >= 1 => {
                    let local_name = name;
                    let local_type =
                        formatter.emit_type(module_id, type_index, &function.namespace)?;

                    if function.locals.is_empty() && local_name.as_bytes() == b"this" {
                    } else {
                        function.margs.push((local_name, local_type));
                    }
                }

                SymbolData::Constant(ConstantSymbol {
                    managed: _,
                    type_index,
                    value,
                    name,
                }) if depth >= 1 => {
                    let const_name = name;
                    let const_type =
                        formatter.emit_type(module_id, type_index, &function.namespace)?;
                    let const_value = value;

                    function
                        .constants
                        .push((const_name, const_type, const_value));
                }

                SymbolData::Data(DataSymbol {
                    global: _,
                    managed: _,
                    type_index,
                    offset,
                    name,
                }) if depth >= 1 => {
                    let static_name = name;
                    let static_type =
                        formatter.emit_type(module_id, type_index, &function.namespace)?;
                    function.statics.push((
                        static_name,
                        static_type,
                        offset.to_rva(address_map).unwrap_or(pdb::Rva(0)),
                    ));
                }

                // Skip
                SymbolData::FrameProcedure(_) => (),

                // Blocks inside functions
                SymbolData::Block(BlockSymbol {
                    parent: _,
                    end: _,
                    len: _,
                    offset,
                    name: _,
                }) if depth >= 1 => {
                    let rva = offset.to_rva(address_map).expect("invalid rva");
                    if let Some(st) = function.statements.iter_mut().find(|st| st.rva == rva) {
                        st.depth = depth;
                    } else {
                        function.blocks.push((rva, depth));
                    }

                    depth += 1;
                }

                // Blocks end
                //
                // TODO: Not only functions and blocks can create scopes.
                // As a crutch, this can do, though some functions will be generated incorrectly.
                SymbolData::ScopeEnd => {
                    depth = (depth - 1).max(0);
                }

                // SymbolData::DefRangeRegisterRelative())
                SymbolData::UserDefinedType(udts) => {
                    static PREDEFINED_TYPEDEFS: LazyLock<HashSet<&[u8]>> = LazyLock::new(|| {
                        [
                            // boost
                            "this_type",
                            "self_type",
                            "unspecified_bool_type",
                            "unqualified_type",
                            "allocator_type",
                            // vostok
                            // ???
                            "vtable_type",
                            "functor_type",
                            "policy_type",
                            "base_type",
                            "callback_type",
                            "create_resource_if_no_file_delegate_type",
                            "first_type",
                            "graph_wrapper_type",
                            "implementation_type",
                            "invoker_type",
                            "is_POD_type",
                            "key_type",
                            "mapped_type",
                            "pod_type",
                            "point_ptr_type",
                            "point_type",
                            "pointer_type",
                            "result_type",
                            "reverse_iterator",
                            "service_impl_type",
                            "size_type",
                            "storage_type",
                            "subscribers_type",
                            "void_type",
                            "void_cv_type",
                            //
                            "value_type",
                            "object_type",
                        ]
                        .into_iter()
                        .map(|t| t.as_bytes())
                        .collect()
                    });

                    let name = udts.name.as_bytes();
                    let c = name[0];

                    if depth != 0 {
                        let udts_name = Type::new(&udts.name.to_string(), &function.namespace);
                        let udts_type =
                            formatter.emit_type(module_id, udts.type_index, &function.namespace)?;
                        function.typedefs.push((udts_type, udts_name));
                    } else {
                        // Most of the typedefs are completely useless, since they come from templates
                        // of different libraries and constantly repeat each other.
                        if !PREDEFINED_TYPEDEFS.contains(name)
                            && c != b'_'
                            && c.is_ascii_lowercase()
                            && name.ends_with(b"_type")
                        {
                            let udts_name = Type::new(&udts.name.to_string(), &function.namespace);
                            let udts_type = formatter.emit_type(
                                module_id,
                                udts.type_index,
                                &function.namespace,
                            )?;

                            typedefs.insert((udts_type, udts_name));
                        }
                    }
                }

                SymbolData::CallSiteInfo(pdb::CallSiteInfoSymbol { offset, type_index }) => {
                    let rva = offset.to_rva(address_map).expect("invalid rva");
                    let noname = pdb::RawString::from("<unknown>");

                    let maybe_fn = Type::new(
                        &formatter.emit_function_orig(&noname, module_id, type_index)?,
                        &function.namespace,
                    );
                    function.callsites.push((rva, maybe_fn))
                }

                // Keep everything that we missed but is inside functions
                symbol if depth != 0 => {
                    function.symbols.push(symbol);
                }

                // Ignore everything outside function scope
                _symbol => (),
            }
        }

        Ok(Module { files, typedefs })
    }

    fn update_cache(&self, cache: &mut FunctionCache, flags: GenFlags) {
        if !flags.contains(GenFlags::NO_CACHE) {
            for funs in self.files.values() {
                for fun in funs.values() {
                    cache.insert_from_source(fun);
                }
            }
        }
    }
}

impl<'a> Function<'a> {
    pub fn new(flags: GenFlags) -> Self {
        Self {
            module_id: Default::default(),
            type_index: Default::default(),
            flags,

            name_orig: Default::default(),
            namespace: Default::default(),

            fn_t: type_parser::Function {
                return_type: type_parser::ReturnType::Constructor,
                name: Default::default(),
                arg_types: Default::default(),
                attrs: type_parser::AttributeFlags::empty(),
            },
            offset: Default::default(),

            margs: Default::default(),
            locals: Default::default(),

            proc_start: Default::default(),
            proc_end: Default::default(),
            statements: Default::default(),

            constants: Default::default(),
            statics: Default::default(),

            blocks: Default::default(),
            typedefs: Default::default(),
            callsites: Default::default(),
            symbols: Default::default(),
        }
    }
}

//
// Writing to disk
//

/// # Arguments
/// * `output_path` - Prefix for the full path to which source files should be written.
/// * `source_path` - `full_path` = `output_path` + `source_path`
impl<'a> Module<'a> {
    fn write(
        self,
        output_path: &mut std::path::PathBuf,
        source_path: &mut std::path::PathBuf,
        engine_path: &str,
        files: &mut Files,
    ) -> crate::Result<()> {
        let output_path_len = output_path.as_path().as_os_str().len();
        let source_path_len = source_path.as_path().as_os_str().len();

        for (file, funs) in self.files {
            let Some(path_to_file) = file.strip_prefix(engine_path) else {
                continue;
            };

            let extension = match () {
                () if path_to_file.ends_with(".h") => ".h",
                () if path_to_file.ends_with(".cpp") => ".cpp",
                () => "",
            };

            source_path.push(path_to_file);
            let mut file = utils_fs::open_file(output_path, source_path, files, extension)?;

            //
            //
            //

            let namespace = assume_namespace(&funs);

            write_header(&mut file, path_to_file)?;
            namespace.start_namespace(&mut file)?;

            for function in funs.into_values() {
                function.write(&namespace, &mut file)?;
            }

            Self::write_typedefs(&self.typedefs, &mut file)?;

            namespace.end_namespace(&mut file)?;
            write_footer(&mut file, path_to_file)?;

            output_path.as_mut_os_string().truncate(output_path_len);
            source_path.as_mut_os_string().truncate(source_path_len);
        }

        Ok(())
    }

    fn write_typedefs(
        typedefs: &BTreeSet<(Type, Type)>,
        mut w: impl std::io::Write,
    ) -> std::io::Result<()> {
        if !typedefs.is_empty() {
            writeln!(w, "\t// TYPEDEFS")?;
            for (ty, name) in typedefs {
                writeln!(w, "\t// typedef")?;
                writeln!(w, "\t// \t{ty}")?;
                writeln!(w, "\t// \t{name};")?;
                writeln!(w)?;
            }
            writeln!(w, "\t// ******\n")?;
        }
        Ok(())
    }
}

impl<'a> Function<'a> {
    pub fn write(self, namespace: &Namespace, mut w: impl std::io::Write) -> std::io::Result<()> {
        let Self {
            module_id: _,
            type_index: _,
            //
            flags,
            offset,
            //
            fn_t,
            name_orig: _,
            namespace: _,
            //
            margs,
            locals,
            //
            proc_start,
            proc_end,
            mut statements,
            //
            constants,
            statics,
            //
            blocks,
            typedefs,
            callsites,
            symbols,
        } = self;

        let margs = margs
            .into_iter()
            .map(|(name, type_)| (name.to_string().to_string(), type_))
            .collect::<Vec<_>>();
        match flags.contains(GenFlags::AS_BASE) {
            true => writeln!(w, "// STUB GENERATED FOR BASE CODE")?,
            false => writeln!(w, "// STATE[STUB]")?,
        }

        // sushi@TODO: The information on whether the function is virtual or not is stored in the
        // class definitions, which are parsed in `gen_hearders.rs`.
        //
        // Since we write functions as we parse them, we don't have this information yet.
        // The proper fix would split parsing and writing into two stages.
        if fn_t.attrs.contains(AttributeFlags::IS_INLINE)
        // && !fn_t.attrs.contains(AttributeFlags::IS_VIRTUAL)
        {
            write!(w, "inline ")?;
        }

        formatter::Formatter::Source
            .write_fn_signature_with_args(&fn_t, namespace, &margs, &mut w)?;

        writeln!(w, "\n{{")?;

        if !locals.is_empty() {
            writeln!(w, "\t// LOCALS")?;
            for (local_name, local_type, local_scope) in locals {
                let local_prefix_len = "// ".len() + local_type.len() + " ".len();

                write!(w, "\t// {local_type} ")?;
                formatter::pad_spaces(&mut w, local_prefix_len)?;
                write!(w, "{local_name}")?;

                if local_scope != 0 {
                    write!(w, "<{local_scope}>")?;
                }
                writeln!(w)?;
            }
            writeln!(w, "\t// ******\n")?;
        }

        if !constants.is_empty() {
            writeln!(w, "\t// CONSTANTS")?;
            for (const_name, const_type, const_value) in constants {
                let const_prefix_len = "// const ".len() + const_type.len() + " ".len();

                write!(w, "\t// const {const_type} ")?;
                formatter::pad_spaces(&mut w, const_prefix_len)?;
                writeln!(w, "{const_name} = {const_value};")?;
            }
            writeln!(w, "\t// ******\n")?;
        }

        if !statics.is_empty() {
            writeln!(w, "\t// STATICS")?;
            for (static_name, static_type, static_rva) in statics {
                let static_prefix_len = "// static ".len() + static_type.len() + " ".len();

                write!(w, "\t// static {static_type} ")?;
                formatter::pad_spaces(&mut w, static_prefix_len)?;
                writeln!(
                    w,
                    "{static_name} = <{offset}>;",
                    offset = static_rva.saturating_add(GAME_IB),
                )?;
            }
            writeln!(w, "\t// ******\n")?;
        }

        if !blocks.is_empty() {
            writeln!(w, "\t// SKIPPED BLOCKS")?;
            for (rva, depth) in blocks {
                writeln!(
                    w,
                    "\t// <{offset}><{depth}>",
                    offset = rva.saturating_add(GAME_IB),
                )?;
            }
            writeln!(w, "\t// ******\n")?;
        }

        if !typedefs.is_empty() {
            writeln!(w, "\t// TYPEDEFS")?;
            for (ty, name) in typedefs {
                writeln!(w, "\t// typedef")?;
                writeln!(w, "\t// \t{ty}")?;
                writeln!(w, "\t// \t{name};")?;
                writeln!(w)?;
            }
            writeln!(w, "\t// ******\n")?;
        }

        if !callsites.is_empty() {
            writeln!(w, "\t// CALL SITE INFO")?;
            for (rva, ty) in callsites {
                writeln!(
                    w,
                    "\t// <{offset}> -> {ty}",
                    offset = rva.saturating_add(GAME_IB),
                )?;
            }
            writeln!(w, "\t// ******\n")?;
        }

        if !symbols.is_empty() {
            writeln!(w, "\t// OTHER SYMBOLS")?;
            for symbol in symbols {
                writeln!(w, "\t// {symbol:?}")?;
            }
            writeln!(w, "\t// ******\n")?;
        }

        if let type_parser::ReturnType::Type(type_) = &fn_t.return_type {
            #[rustfmt::skip]
            let return_value = match type_.as_str() {
                _ if type_.ends_with('*')    => "NULL",
                "pcstr"                      => "NULL",

                "vostok::math::aabb"         => "vostok::math::aabb()",
                "vostok::math::color"        => "vostok::math::color()",
                "vostok::math::frustum"      => "vostok::math::frustum()",
                "vostok::math::intersection" => "vostok::math::intersection()",
                "vostok::math::plane"        => "vostok::math::plane()",
                "vostok::math::quaternion"   => "vostok::math::quaternion()",

                "vostok::math::uint2"        => "vostok::math::uint2(1, 1)",

                "vostok::math::float2"       => "vostok::math::float2(1., 1.)",
                "vostok::math::float3"       => "vostok::math::float3(1., 1., 1.)",
                "vostok::math::float4"       => "vostok::math::float4(1., 1., 1., 1.)",

                "vostok::math::float3_pod"   => "vostok::math::float3_pod()",
                "vostok::math::float4_pod"   => "vostok::math::float4_pod()",
                "vostok::math::float4x4"     => "vostok::math::float4x4()",

                // sad
                "math::aabb"         => "math::aabb()",
                "math::color"        => "math::color()",
                "math::frustum"      => "math::frustum()",
                "math::intersection" => "math::intersection()",
                "math::plane"        => "math::plane()",
                "math::quaternion"   => "math::quaternion()",

                "uint2"        => "uint2(1, 1)",

                "float2"       => "float2(1., 1.)",
                "float3"       => "float3(1., 1., 1.)",
                "float4"       => "float4(1., 1., 1., 1.)",

                "float3_pod"   => "float3_pod()",
                "float4_pod"   => "float4_pod()",
                "float4x4"     => "float4x4()",
                // sad end


                "u8" | "u16" | "u32" => "0",
                "s8" | "s16" | "s32" => "0",
                "float" | "double"   => "0.0f",
                "bool"               => "false",
                "char"               => "'a'",

                "void" => "",
                _      => "",
            };

            if !return_value.is_empty() {
                writeln!(w, "\treturn {return_value};\n")?;
            }
        }

        {
            statements.sort_by_key(|statement| statement.line_start);
            let floc = match statements.len() < 2 {
                true => None,
                false => Some(
                    statements.last().unwrap().line_start as i32
                        - statements.first().unwrap().line_start as i32
                        - 1,
                ),
            };

            let offset = offset.saturating_add(GAME_IB);
            match floc {
                None => writeln!(w, "\t// FUNCTION BODY[{offset}]")?,
                Some(loc) => writeln!(w, "\t// FUNCTION BODY[{offset}]: {loc}")?,
            }

            let rva_diff = |lhs: pdb::Rva, rhs: pdb::Rva| -> i32 { lhs.0 as i32 - rhs.0 as i32 };
            let print_rva_diff_start = |diff: i32| match diff >= 0 {
                true => format!("0x{diff:03x}"),
                false => format!("-0x{diff:03x}", diff = diff.abs()),
            };
            let print_rva_diff_next = |diff: Option<i32>| match diff {
                None => "      ".to_string(),
                Some(diff) => match diff >= 0 {
                    true => format!("+0x{diff:03x}"),
                    false => format!("-0x{diff:03x}", diff = diff.abs()),
                },
            };

            let mut non_empty_body = statements.len() > 2;

            // Sometimes there are multiple statements for the end `}` line.
            // When this happens, do not hide brackets
            if non_empty_body {
                let len = statements.len();
                if statements[len - 2].line_start == statements[len - 1].line_start {
                    non_empty_body = false;
                }

                if statements[0].line_start == statements[1].line_start {
                    non_empty_body = false;
                }
            }

            let mut next_line = proc_start;
            for i in 0..statements.len() {
                let Statement {
                    rva,
                    line_start,
                    depth,
                } = statements[i];

                // There can be multiple statements on a single line
                for empty_line_no in 0..line_start.saturating_sub(next_line) {
                    writeln!(w, "\t// <{empty_line_no}>")?
                }
                next_line = line_start + 1;

                let diff_start = rva_diff(rva, statements[0].rva);
                let diff_next = statements
                    .get(i + 1)
                    .map(|statement| rva_diff(statement.rva, rva));
                let offset = rva.saturating_add(GAME_IB);

                let diff_start = print_rva_diff_start(diff_start);
                let diff_next = print_rva_diff_next(diff_next);

                let suffix = if i == 0 {
                    "\t{"
                } else if i == statements.len() - 1 {
                    "\t}"
                } else {
                    ""
                };

                if !suffix.is_empty() && non_empty_body {
                    continue;
                }

                #[rustfmt::skip]
                match depth {
                    0  => writeln!(w, "\t// <{offset}>|{diff_start}|{diff_next}:'{line_start}'{suffix}"),
                    _  => writeln!(w, "\t// <{offset}>|{diff_start}|{diff_next}|[{depth}]:'{line_start}'{suffix}"),
                }?;
            }

            for empty_line_no in 0..proc_end.saturating_sub(next_line) {
                writeln!(w, "\t// <{empty_line_no}>")?
            }

            writeln!(w, "\t// ******")?;
        }

        writeln!(w, "}}")?;

        writeln!(w)?;

        Ok(())
    }
}

pub fn write_header(w: &mut impl std::io::Write, path: &str) -> crate::Result<()> {
    use chrono::Local;
    let day = Local::now().format("%d.%m.%Y").to_string();

    #[rustfmt::skip]
    {
        writeln!(w, "////////////////////////////////////////////////////////////////////////////")?;
        writeln!(w, "//	Created 	: {day}")?;
        writeln!(w, "////////////////////////////////////////////////////////////////////////////")?;
        writeln!(w)?;
    };

    let file_name = Path::new(path)
        .file_name()
        .expect("no filename")
        .to_string_lossy()
        .to_string();
    if let Some(module_name) = file_name.strip_suffix(".cpp") {
        writeln!(w, "#include \"pch.h\"")?;
        writeln!(w, "#include \"{module_name}.h\"")?;
        writeln!(w)?;
    } else if let Some(module_name) = file_name.strip_suffix(".h") {
        let ifdef = format!("{}_H_INCLUDED", module_name.to_uppercase());

        writeln!(w, "#ifndef {ifdef}")?;
        writeln!(w, "#define {ifdef}")?;
        writeln!(w)?;
    }

    Ok(())
}

pub fn write_footer(w: &mut impl std::io::Write, path: &str) -> crate::Result<()> {
    let file_name = Path::new(path)
        .file_name()
        .expect("no filename")
        .to_string_lossy()
        .to_string();

    if let Some(module_name) = file_name.strip_suffix(".h") {
        writeln!(w)?;

        let ifdef = format!("{}_H_INCLUDED", module_name.to_uppercase());

        writeln!(w, "#endif // #ifndef {ifdef}")?;
    }
    Ok(())
}

fn assume_namespace(funs: &BTreeMap<u32, Function>) -> Namespace {
    let mut namespace = Namespace::default();

    for fun in funs.values() {
        if fun.namespace != namespace && fun.namespace.depth() > namespace.depth() {
            namespace = fun.namespace.clone();
        }
    }

    namespace
}
