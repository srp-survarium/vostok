use std::collections::{btree_map, BTreeMap, BTreeSet, HashMap};
use std::io::{self, BufWriter};
use std::path::Path;

use pdb::{FallibleIterator, ItemIndex};
use pdb_addr2line::type_parser;
use pdb_addr2line::type_parser::AttributeFlags;
use pdb_addr2line::type_parser::ReturnType;

use crate::helpers::{Files, FunctionCache, FunctionSignature};
use crate::pdb_parser::PdbParser;
use crate::GenFlags;
use crate::{Namespace, Type};

use crate::{formatter, gen_sources, pdb_parser, type_builder, utils_fs};

pub fn dump_headers(
    pdb: &mut pdb::PDB<std::fs::File>,
    formatter: &PdbParser,
    cache: FunctionCache,
    output_path_prefix: &std::path::Path,
    flags: GenFlags,
    files: &mut Files,
) -> crate::Result<()> {
    let type_information = pdb.type_information()?;
    let type_finder = {
        let mut type_finder = type_information.finder();

        let mut type_iter = type_information.iter();
        while type_iter.next()?.is_some() {
            type_finder.update(&type_iter);
        }
        type_finder
    };

    let mut output_path = output_path_prefix.to_path_buf();
    let mut header_path = Path::new("headers").to_path_buf();

    let output_path_len = output_path.as_path().as_os_str().len();
    let header_path_len = header_path.as_path().as_os_str().len();

    let type_information = pdb.type_information()?;
    let mut type_iter = type_information.iter();

    // Extract all enums out of the classes in which they are defined and then create headers for them.
    //
    // Note that sometimes the same enum can be specified with and without fields in headers.
    // When this happens, fieldless enum will be replaced.
    let mut enums = BTreeMap::new();

    while let Some(type_index) = type_iter.next()? {
        let Ok(pdb::TypeData::Class(class)) = type_index.parse() else {
            continue;
        };
        if class.properties.forward_reference() {
            continue;
        }

        let namespace = Namespace::get_from_class_name_impl(&class.name.to_string());

        let Ok(header) = build_header(
            formatter,
            &cache,
            &type_finder,
            type_index.index(),
            &namespace,
            &mut enums,
        ) else {
            continue;
        };

        if let Some(file) = create_header_file(
            &class.name,
            &namespace,
            HeaderType::Class,
            &mut output_path,
            &mut header_path,
            flags,
            files,
        )? {
            let mut file = BufWriter::new(file);
            header.write_to_header_file(&class.name.to_string(), &mut file)?;
        }

        output_path.as_mut_os_string().truncate(output_path_len);
        header_path.as_mut_os_string().truncate(header_path_len);
    }

    for e in enums.values() {
        let enum_name = &e.name.to_string();
        let namespace = Namespace::get_from_class_name_impl(enum_name);

        if let Some(file) = create_header_file(
            &e.name,
            &namespace,
            HeaderType::Enum,
            &mut output_path,
            &mut header_path,
            flags,
            files,
        )? {
            let mut file = BufWriter::new(file);
            e.write_to_header_file(enum_name, &namespace, &mut file)?;
        }

        output_path.as_mut_os_string().truncate(output_path_len);
        header_path.as_mut_os_string().truncate(header_path_len);
    }

    Ok(())
}

fn build_header<'pdb>(
    formatter: &PdbParser,
    cache: &FunctionCache,
    type_finder: &pdb::TypeFinder<'pdb>,

    class: pdb::TypeIndex,
    namespace: &Namespace,

    enums: &mut BTreeMap<pdb::RawString<'pdb>, Enum<'pdb>>,
) -> crate::Result<Data<'pdb>> {
    let mut needed_types = TypeSet::new();
    let mut data = Data::new(namespace.clone());

    data.add(
        formatter,
        cache,
        type_finder,
        class,
        &mut needed_types,
        enums,
    )?;

    // add all the needed types iteratively until we're done
    while let Some(type_index) = needed_types.iter().next_back().copied() {
        // remove it
        needed_types.remove(&type_index);

        // add the type
        data.add(
            formatter,
            cache,
            type_finder,
            type_index,
            &mut needed_types,
            enums,
        )?;
    }

    Ok(data)
}

//
//
//

type TypeSet = BTreeSet<pdb::TypeIndex>;

struct Data<'p> {
    namespace: Namespace,
    forward_references: HashMap<String, ForwardReference>,
    classes: Vec<Class<'p>>,
    enums: Vec<Enum<'p>>,
}

struct Class<'p> {
    namespace: Namespace,
    kind: pdb::ClassKind,
    orig_name: String,
    name: Type,
    size: u64,
    base_classes: Vec<BaseClass>,
    fields: Vec<Field<'p>>,
    instance_methods: Vec<Method>,
    static_methods: Vec<Method>,
}

struct BaseClass {
    type_name: Type,
    offset: u32,
}

struct Field<'p> {
    type_name: Type,
    name: pdb::RawString<'p>,
    array: String,
    offset: u64,
}

enum Method {
    FromHeaderFile {
        fn_t: type_parser::Function,
    },
    FromSourceFile {
        fn_t: type_parser::Function,
        margs: Vec<(String, Type)>,
    },
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct Enum<'p> {
    name: pdb::RawString<'p>,
    underlying_type_name: Type,
    values: Vec<EnumValue<'p>>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct EnumValue<'p> {
    name: pdb::RawString<'p>,
    value: pdb::Variant,
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct ForwardReference {
    kind: ForwardReferenceKind,
    usage: ForwardReferenceUsage,
    name: String,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
enum ForwardReferenceKind {
    Class,
    Struct,
    Enum,
    Unknown,
    Typedef,
    TypedefInner,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
enum ForwardReferenceUsage {
    ByValue,
    ByReference,
}

//
//
//

impl<'pdb> Data<'pdb> {
    fn new(namespace: Namespace) -> Data<'pdb> {
        Data {
            namespace,
            forward_references: HashMap::new(),
            classes: Vec::new(),
            enums: Vec::new(),
        }
    }

    fn add(
        &mut self,

        formatter: &PdbParser,
        cache: &FunctionCache,
        type_finder: &pdb::TypeFinder<'pdb>,

        type_index: pdb::TypeIndex,
        needed_types: &mut TypeSet,

        enums: &mut BTreeMap<pdb::RawString<'pdb>, Enum<'pdb>>,
    ) -> crate::Result<()> {
        match type_finder.find(type_index)?.parse()? {
            pdb::TypeData::Class(data) => {
                let orig_name = data.name.to_string().to_string();

                let namespace = &self.namespace;

                if data.properties.forward_reference() {
                    let name = data.name.to_string();

                    // Skip forward references which are always present through `pch.h`.
                    let Some(name) = skip_default_environment(&name) else {
                        return Ok(());
                    };

                    match type_builder::extract_forward_reference_from_template(name) {
                        None => {
                            _ = self.forward_references.insert(
                                name.to_string(),
                                ForwardReference {
                                    kind: ForwardReferenceKind::from_class_kind(data.kind),
                                    usage: ForwardReferenceUsage::ByValue,
                                    name: name.to_string(),
                                },
                            )
                        }
                        Some(targ) => {
                            self.forward_references.insert(
                                targ.to_string(),
                                ForwardReference {
                                    kind: ForwardReferenceKind::TypedefInner,
                                    usage: ForwardReferenceUsage::ByValue,
                                    name: targ.to_string(),
                                },
                            );

                            match type_builder::typedef_template(name) {
                                Some(typedef) => {
                                    let name = format!("{name}\n\t{typedef}");
                                    _ = self.forward_references.insert(
                                        name.to_string(),
                                        ForwardReference {
                                            kind: ForwardReferenceKind::Typedef,
                                            usage: ForwardReferenceUsage::ByValue,
                                            name: name.to_string(),
                                        },
                                    )
                                }
                                None => {
                                    _ = self.forward_references.insert(
                                        name.to_string(),
                                        ForwardReference {
                                            kind: ForwardReferenceKind::TypedefInner,
                                            usage: ForwardReferenceUsage::ByValue,
                                            name: name.to_string(),
                                        },
                                    )
                                }
                            }
                        }
                    }

                    return Ok(());
                }

                let mut class = Class {
                    namespace: namespace.clone(),
                    kind: data.kind,
                    name: Type::new(&data.name.to_string(), namespace),
                    orig_name,
                    size: data.size,
                    fields: Vec::new(),
                    base_classes: Vec::new(),
                    instance_methods: Vec::new(),
                    static_methods: Vec::new(),
                };

                if let Some(fields) = data.fields {
                    class.add_fields(
                        formatter,
                        cache,
                        type_finder,
                        fields,
                        needed_types,
                        &mut self.forward_references,
                    )?;
                }

                self.classes.insert(0, class);
            }

            pdb::TypeData::Enumeration(data) => {
                let mut e = Enum {
                    name: data.name,
                    underlying_type_name: type_name(
                        formatter,
                        type_finder,
                        data.underlying_type,
                        needed_types,
                        &self.namespace,
                    )?,
                    values: Vec::new(),
                };

                e.add_fields(type_finder, data.fields, needed_types)?;

                self.enums.insert(0, e.clone());
                match enums.entry(e.name) {
                    btree_map::Entry::Vacant(entry) => _ = entry.insert(e),
                    btree_map::Entry::Occupied(mut entry) => {
                        match (entry.get().values.len(), e.values.len()) {
                            (lhs, rhs) if lhs == rhs => (),
                            (_, 0) => (),
                            (0, _) => _ = entry.insert(e),
                            _ if e.name.as_bytes() == b"ARG_TYPE" => (),
                            _ => unreachable!("Enums cannot be of different length"),
                        }
                    }
                }
            }

            pdb::TypeData::Union(_) => (/* TODO */),

            // ignore
            other => eprintln!("warning: don't know how to add {other:?}"),
        }

        Ok(())
    }
}

impl<'p> Class<'p> {
    fn add_fields(
        &mut self,
        formatter: &PdbParser,
        cache: &FunctionCache,
        type_finder: &pdb::TypeFinder<'p>,

        type_index: pdb::TypeIndex,
        needed_types: &mut TypeSet,
        forward_references: &mut HashMap<String, ForwardReference>,
    ) -> crate::Result<()> {
        match type_finder.find(type_index)?.parse()? {
            pdb::TypeData::FieldList(data) => {
                for field in &data.fields {
                    self.add_field(
                        formatter,
                        cache,
                        type_finder,
                        field,
                        needed_types,
                        forward_references,
                    )?;
                }

                if let Some(continuation) = data.continuation {
                    // recurse
                    self.add_fields(
                        formatter,
                        cache,
                        type_finder,
                        continuation,
                        needed_types,
                        forward_references,
                    )?;
                }
            }
            other => {
                eprintln!("trying to Class::add_fields() got {type_index} -> {other:?}");
                panic!("unexpected type in Class::add_fields()");
            }
        }

        Ok(())
    }

    fn add_field(
        &mut self,

        formatter: &PdbParser,
        cache: &FunctionCache,
        type_finder: &pdb::TypeFinder<'p>,

        field: &pdb::TypeData<'p>,
        needed_types: &mut TypeSet,
        forward_references: &mut HashMap<String, ForwardReference>,
    ) -> crate::Result<()> {
        match *field {
            pdb::TypeData::Member(ref data) => {
                self.fields.push(Field::build(
                    type_name(
                        formatter,
                        type_finder,
                        data.field_type,
                        needed_types,
                        &self.namespace,
                    )?,
                    data.name,
                    data.offset,
                ));
            }

            pdb::TypeData::Method(ref data) => self.add_method(
                formatter,
                cache,
                type_finder,
                data.name,
                data.attributes,
                data.method_type,
                forward_references,
            )?,

            pdb::TypeData::OverloadedMethod(ref data) => {
                // this just means we have more than one method with the same name
                // find the method list
                match type_finder.find(data.method_list)?.parse()? {
                    pdb::TypeData::MethodList(method_list) => {
                        // let mut methods = method_list.methods.clone();
                        // methods.sort_by_key(|method| method.vtable_offset);

                        for pdb::MethodListEntry {
                            attributes,
                            method_type,
                            ..
                        } in method_list.methods.into_iter().rev()
                        {
                            // hooray
                            self.add_method(
                                formatter,
                                cache,
                                type_finder,
                                data.name,
                                attributes,
                                method_type,
                                forward_references,
                            )?;
                        }
                    }
                    other => {
                        eprintln!(
                            "processing OverloadedMethod, expected MethodList, got {} -> {other:?}",
                            data.method_list,
                        );
                        panic!("unexpected type in Class::add_field()");
                    }
                }
            }

            pdb::TypeData::BaseClass(ref data) => self.base_classes.push(BaseClass {
                type_name: type_name(
                    formatter,
                    type_finder,
                    data.base_class,
                    needed_types,
                    &self.namespace,
                )?,
                offset: data.offset,
            }),

            pdb::TypeData::VirtualBaseClass(ref data) => self.base_classes.push(BaseClass {
                type_name: type_name(
                    formatter,
                    type_finder,
                    data.base_class,
                    needed_types,
                    &self.namespace,
                )?,
                offset: data.base_pointer_offset,
            }),

            _ => {
                // ignore everything else even though that's sad
            }
        }

        Ok(())
    }

    #[expect(clippy::too_many_arguments)]
    fn add_method(
        &mut self,

        formatter: &PdbParser,
        cache: &FunctionCache,
        type_finder: &pdb::TypeFinder<'p>,

        data_name: pdb::RawString,
        data_attributes: pdb::FieldAttributes,
        data_method_type: pdb::TypeIndex,

        forward_references: &mut HashMap<String, ForwardReference>,
    ) -> crate::Result<()> {
        let method = Method::find(
            formatter,
            cache,
            type_finder,
            &self.orig_name,
            data_name,
            data_attributes,
            data_method_type,
        )?;

        let fn_t = method.fn_t();
        let copy_arg = |postfix: &str| {
            fn_t.arg_types[0].strip_suffix(postfix).unwrap_or_default() == self.orig_name
        };

        match fn_t.name.as_str() {
            // Vector destructor generated by compiler
            "__vecDelDtor" => return Ok(()),

            // Copy assignment generated by compiler
            fn_name
                // Constructor doesn't have definition
                if matches!(method, Method::FromHeaderFile { .. })
                    // The constructor has a single argument only
                    && fn_t.arg_types.len() == 1
                    // Which is a reference to original type
                    && (copy_arg(" const&") || copy_arg("&"))
                    // Which is either constructor or assignment
                    && (fn_name == "operator="
                        || matches!(fn_t.return_type, ReturnType::Constructor)) =>
            {
                return Ok(());
            }

            "__local_vftable_ctor_closure" => return Ok(()),
            "__dflt_ctor_closure" => return Ok(()),

            _ => (),
        }

        for arg_ty in &fn_t.arg_types {
            if let Some(forward_reference) =
                find_forward_reference(arg_ty, &self.name, &self.namespace)
            {
                forward_references
                    .entry(forward_reference.name.clone())
                    .or_insert(forward_reference);
            }
        }
        if let ReturnType::Type(ret_ty) = &fn_t.return_type {
            if let Some(forward_reference) =
                find_forward_reference(ret_ty, &self.name, &self.namespace)
            {
                forward_references
                    .entry(forward_reference.name.clone())
                    .or_insert(forward_reference);
            }
        }

        if data_attributes.is_static() {
            self.static_methods.push(method);
        } else {
            self.instance_methods.push(method);
        }

        Ok(())
    }
}

impl Method {
    fn find(
        formatter: &PdbParser,
        cache: &FunctionCache,
        type_finder: &pdb::TypeFinder,

        class_name: &str,
        name: pdb::RawString,
        attributes: pdb::FieldAttributes,
        type_index: pdb::TypeIndex,
    ) -> crate::Result<Method> {
        match type_finder.find(type_index)?.parse()? {
            pdb::TypeData::MemberFunction(_) => {
                assert!(!type_index.is_cross_module());

                let mut method =
                    match cache.get_from_header(class_name, &name, formatter, type_index)? {
                        None => Method::FromHeaderFile {
                            fn_t: formatter.parse_function(&name, 0, type_index)?,
                        },
                        Some(FunctionSignature { fn_t, margs }) => {
                            Method::FromSourceFile { fn_t, margs }
                        }
                    };

                method.set_method_attributes(attributes);

                Ok(method)
            }

            other => {
                eprintln!("other: {other:?}");
                Err(pdb::Error::UnimplementedFeature("that").into())
            }
        }
    }

    fn set_method_attributes(&mut self, attrs: pdb::FieldAttributes) {
        match self {
            Self::FromHeaderFile { fn_t } => pdb_parser::set_method_attributes(fn_t, attrs, false),
            Self::FromSourceFile { fn_t, .. } => {
                pdb_parser::set_method_attributes(fn_t, attrs, true)
            }
        }
    }
}

impl<'p> Enum<'p> {
    fn add_fields(
        &mut self,
        type_finder: &pdb::TypeFinder<'p>,
        type_index: pdb::TypeIndex,
        needed_types: &mut TypeSet,
    ) -> crate::Result<()> {
        match type_finder.find(type_index)?.parse()? {
            pdb::TypeData::FieldList(data) => {
                for field in &data.fields {
                    self.add_field(type_finder, field, needed_types);
                }

                if let Some(continuation) = data.continuation {
                    // recurse
                    self.add_fields(type_finder, continuation, needed_types)?;
                }
            }

            pdb::TypeData::Primitive(pdb::PrimitiveType {
                kind: pdb::PrimitiveKind::NoType,
                ..
            }) => (),

            other => {
                println!("trying to Enum::add_fields() got {type_index} -> {other:?}");
                panic!("unexpected type in Enum::add_fields()");
            }
        }

        Ok(())
    }

    fn add_field(&mut self, _: &pdb::TypeFinder<'p>, field: &pdb::TypeData<'p>, _: &mut TypeSet) {
        // ignore everything else even though that's sad
        if let pdb::TypeData::Enumerate(data) = &field {
            self.values.push(EnumValue {
                name: data.name,
                value: data.value,
            });
        }
    }
}

impl<'p> Field<'p> {
    pub fn build(mut type_name: Type, name: pdb::RawString<'p>, offset: u64) -> Self {
        let mut array = String::new();
        if let Some(pos) = type_name.0.find('[') {
            array = type_name.0.split_at(pos).1.to_string();
            type_name.0.truncate(pos);
        }

        Self {
            type_name,
            name,
            array,
            offset,
        }
    }
}

//
//
//

pub fn type_name(
    formatter: &PdbParser,
    type_finder: &pdb::TypeFinder<'_>,
    type_index: pdb::TypeIndex,
    needed_types: &mut TypeSet,
    namespace: &Namespace,
) -> crate::Result<Type> {
    update_referenced_types(type_finder, type_index, needed_types)?;

    // Make sure that index is not cross module.
    // That means it can be easily resolved.
    assert!(!type_index.is_cross_module());
    formatter.emit_type(0, type_index, namespace)
}

pub fn update_referenced_types(
    type_finder: &pdb::TypeFinder<'_>,
    type_index: pdb::TypeIndex,
    needed_types: &mut TypeSet,
) -> crate::Result<()> {
    match type_finder.find(type_index)?.parse()? {
        pdb::TypeData::Class(_) => {
            needed_types.insert(type_index);
        }

        pdb::TypeData::Enumeration(_) => {
            needed_types.insert(type_index);
        }

        pdb::TypeData::Union(_) => {
            needed_types.insert(type_index);
        }

        pdb::TypeData::Pointer(data) => {
            update_referenced_types(type_finder, data.underlying_type, needed_types)?
        }

        pdb::TypeData::Modifier(data) => {
            update_referenced_types(type_finder, data.underlying_type, needed_types)?
        }

        pdb::TypeData::Array(data) => {
            update_referenced_types(type_finder, data.element_type, needed_types)?
        }

        _ => (),
    }

    Ok(())
}

//
//
//

impl Data<'_> {
    fn write_to_header_file(
        &self,
        class_name: &str,
        f: &mut impl std::io::Write,
    ) -> crate::Result<()> {
        let ifdef_name = get_ifdef_name(class_name);

        gen_sources::write_header(f, &ifdef_name)?;

        if !self.forward_references.is_empty() {
            let mut by_value = self
                .forward_references
                .values()
                .filter(|r| r.usage == ForwardReferenceUsage::ByValue)
                .collect::<Vec<_>>();

            let mut by_reference = self
                .forward_references
                .values()
                .filter(|r| r.usage == ForwardReferenceUsage::ByReference)
                .collect::<Vec<_>>();

            let sort = |lhs: &&ForwardReference, rhs: &&ForwardReference| {
                use core::cmp::Ordering;

                let engine_name =
                    |name: &str| name.starts_with("vostok") || name.starts_with("survarium");

                match (engine_name(&lhs.name), engine_name(&rhs.name)) {
                    (false, true) => return Ordering::Less,
                    (true, false) => return Ordering::Greater,
                    _ => (),
                }

                if lhs.kind != rhs.kind {
                    return lhs.kind.cmp(&rhs.kind);
                }

                if lhs.name.starts_with("vostok") && rhs.name.starts_with("survarium") {
                    return Ordering::Less;
                }

                if lhs.name.starts_with("survarium") && rhs.name.starts_with("vostok") {
                    return Ordering::Greater;
                }

                lhs.name.cmp(&rhs.name)
            };

            by_value.sort_by(sort);
            by_reference.sort_by(sort);

            if !by_value.is_empty() {
                writeln!(f, "/* INCLUDES */")?;

                for e in by_value {
                    e.fmt(f)?;
                }

                if !by_reference.is_empty() {
                    writeln!(f)?
                }
            }

            if !by_reference.is_empty() {
                writeln!(f, "/* FORWARD REFS */")?;

                for e in by_reference {
                    e.fmt(f)?;
                }
            }

            writeln!(f)?;
        }

        self.namespace.start_namespace(f)?;
        self.write(f)?;
        self.namespace.end_namespace(f)?;

        gen_sources::write_footer(f, &ifdef_name)?;

        Ok(())
    }
}

impl Enum<'_> {
    fn write_to_header_file(
        &self,
        enum_name: &str,
        namespace: &Namespace,
        f: &mut impl std::io::Write,
    ) -> crate::Result<()> {
        let ifdef_name = get_ifdef_name(enum_name);

        gen_sources::write_header(f, &ifdef_name)?;
        namespace.start_namespace(f)?;
        self.fmt(Name::RemoveNamespace(namespace), f)?;
        writeln!(f)?;
        namespace.end_namespace(f)?;
        gen_sources::write_footer(f, &ifdef_name)?;

        Ok(())
    }
}

fn get_ifdef_name(name: &str) -> String {
    let mut depth = 0;

    let header_name = name.chars().filter(|c| match c {
        '<' => {
            depth += 1;
            false
        }
        '>' => {
            depth -= 1;
            false
        }
        _ => depth == 0,
    });

    "ignore/"
        .chars()
        .chain(header_name)
        .chain(".h".chars())
        .collect::<String>()
        .replace("survarium::", "")
        .replace("vostok::", "")
        .replace("::", "_")
}

//
// Display
//

impl Data<'_> {
    fn write(&self, f: &mut impl std::io::Write) -> io::Result<()> {
        for e in &self.enums {
            e.fmt(Name::Full, f)?;
        }

        if !self.enums.is_empty() {
            writeln!(f)?;
        }

        for class in &self.classes {
            class.fmt(f)?;
        }

        Ok(())
    }
}

impl Class<'_> {
    fn fmt(&self, f: &mut impl std::io::Write) -> io::Result<()> {
        let kind = match self.kind {
            pdb::ClassKind::Class => "class",
            pdb::ClassKind::Struct => "struct",
            pdb::ClassKind::Interface => "interface",
        };
        let name = &self.name;
        write!(f, "{kind} {name}")?;

        if !self.base_classes.is_empty() {
            for (i, base) in self.base_classes.iter().enumerate() {
                let prefix = match i {
                    0 => ":",
                    _ => ",",
                };
                write!(f, " {prefix} public {}", base.type_name)?;
            }
        }

        writeln!(f, " {{")?;

        //
        // All methods are considered public
        //
        if !self.instance_methods.is_empty() || !self.static_methods.is_empty() {
            if !matches!(self.kind, pdb::ClassKind::Struct) {
                writeln!(f, "public:")?;
            }

            let max_return_type_len = self.max_return_type_len();
            let max_method_name_len = self.max_method_name_len();

            if !self.instance_methods.is_empty() {
                let mut prev_name = "";

                for method in &self.instance_methods {
                    let name = method.fn_t().name.as_str();
                    #[rustfmt::skip]
                    match (prev_name, name) {
                        ("", _) => (),
                        // Overloads should be grouped
                        (lhs, rhs) if lhs == rhs => (),
                        // Constructor and destructor should be grouped
                        (lhs, rhs) if lhs == &rhs[1..] => (),
                        // get_, on_, register_, unregister_, etc.
                        (lhs, rhs) if starts_with_equal_group(lhs, rhs) => (),
                        // _subscriber, _affect, _callback
                        (lhs, rhs) if ends_with_equal_group(lhs, rhs) => (),

                        // Known pairs to be grouped
                        ("serialize",  "deserialize") => (),
                        ("initialize", "finalize") => (),
                        ("initialize", "destroy") => (),
                        ("insert",     "remove") => (),
                        ("subscribe",  "unsubscribe") => (),
                        ("from",        "to") => (),

                        _ => writeln!(f)?,
                    };

                    method.fmt(
                        f,
                        &self.namespace,
                        self.has_inline_methods(),
                        max_return_type_len,
                        max_method_name_len,
                    )?;

                    prev_name = name;
                }
            }

            if !self.static_methods.is_empty() {
                writeln!(f)?;

                for method in &self.static_methods {
                    method.fmt(
                        f,
                        &self.namespace,
                        false,
                        max_return_type_len,
                        max_method_name_len,
                    )?;
                }
            }
        }

        if !self.fields.is_empty() {
            writeln!(f)?;

            //
            // All fields are considered public unless this is a struct
            //
            match self.kind {
                pdb::ClassKind::Class => writeln!(f, "private:")?,
                pdb::ClassKind::Interface => writeln!(f, "private:")?,
                pdb::ClassKind::Struct => writeln!(f, "public:")?,
            }

            for base in &self.base_classes {
                writeln!(f, "\t/* 0x{:04x} */\t/* {} */", base.offset, base.type_name)?;
            }

            let max_type_name_len = self.max_type_name_len();
            for Field {
                type_name,
                name,
                array,
                offset,
            } in &self.fields
            {
                write!(f, "\t/* 0x{offset:04x} */\t{type_name}")?;
                formatter::pad_spaces_t(f, type_name.len(), max_type_name_len)?;
                writeln!(f, "\t{}{};", name.to_string(), array)?;
            }
        }

        writeln!(f, "}}; // {kind} {name}")?;

        let size = self.size;
        writeln!(f)?;
        writeln!(f, "STATIC_SIZE_ASSERT({name}, 0x{size:X});")?;
        writeln!(f)?;

        Ok(())
    }
}

impl Class<'_> {
    fn has_inline_methods(&self) -> bool {
        self.instance_methods.iter().any(|method| {
            method.attrs().contains(AttributeFlags::IS_INLINE)
                | method.attrs().contains(AttributeFlags::IS_VIRTUAL)
        })
    }

    fn max_type_name_len(&self) -> usize {
        formatter::get_max_length(&self.fields, |field| field.type_name.len())
    }

    fn max_return_type_len(&self) -> usize {
        formatter::get_max_length(&self.instance_methods, |method| {
            formatter::get_return_type(
                &method.fn_t().return_type,
                method.fn_t().arg_types.len(),
                &self.namespace,
            )
            .len()
        })
    }

    fn max_method_name_len(&self) -> usize {
        formatter::get_max_length(&self.instance_methods, |method| method.fn_t().name.len())
    }
}

impl Method {
    fn fmt(
        &self,
        f: &mut impl std::io::Write,
        namespace: &Namespace,
        has_inline_methods: bool,
        max_return_type_len: usize,
        max_method_name_len: usize,
    ) -> io::Result<()> {
        let type_parser::Function {
            attrs, return_type, ..
        } = self.fn_t();

        let virtual_ = match attrs.contains(AttributeFlags::IS_VIRTUAL) {
            true => "virtual\t",
            false => "",
        };
        let (inline, body) = match attrs.contains(AttributeFlags::IS_INLINE) {
            true if attrs.contains(AttributeFlags::IS_VIRTUAL) => ("", " { /* no source */ }"),
            true => ("inline\t", " { /* no source */ }"),
            false => ("", ";"),
        };
        let static_ = match attrs.contains(AttributeFlags::IS_STATIC) {
            true => "static\t",
            false => "",
        };

        let tab_prefix = match inline.is_empty()
            && virtual_.is_empty()
            && static_.is_empty()
            && has_inline_methods
        {
            true => "\t\t",
            false => "",
        };

        let override_ = match attrs.contains(AttributeFlags::IS_OVERRIDE) {
            true if !matches!(return_type, ReturnType::Destructor) => " override",
            _ => "",
        };
        let pure = match attrs.contains(AttributeFlags::IS_PURE) {
            true => " = 0",
            false => "",
        };
        let final_ = match attrs.contains(AttributeFlags::IS_FINAL) {
            true => " final",
            false => "",
        };

        // sushi@TODO: This possibly should be moved inside formatter
        let mut pad_args_len = 4; // \t
        if !(virtual_.is_empty()
            && inline.is_empty()
            && static_.is_empty()
            && tab_prefix.is_empty())
        {
            pad_args_len += 8; // \t\t
        }
        pad_args_len += formatter::pad_times(0, max_return_type_len) * 4 + 4;
        // pad_args_len += utils::pad_times(0, max_method_name_len) * 4;

        write!(f, "\t{virtual_}{static_}{inline}{tab_prefix}")?;

        let header_formatter = formatter::HeaderFormatter {
            max_return_type_len,
            max_method_name_len,
            pad_args_len,
        };

        match self {
            Method::FromHeaderFile { fn_t } => {
                header_formatter.write_fn_signature_unnamed_args(fn_t, namespace, f)?;
            }
            Method::FromSourceFile { fn_t, margs } => {
                formatter::Formatter::Header(header_formatter)
                    .write_fn_signature_with_args(fn_t, namespace, margs, f)?;
            }
        }
        writeln!(f, "{override_}{pure}{final_}{body}")?;

        Ok(())
    }

    fn fn_t(&self) -> &type_parser::Function {
        match self {
            Method::FromHeaderFile { fn_t } => fn_t,
            Method::FromSourceFile { fn_t, .. } => fn_t,
        }
    }

    fn attrs(&self) -> AttributeFlags {
        self.fn_t().attrs
    }
}

enum Name<'a> {
    Full,
    RemoveNamespace(&'a Namespace),
}

impl Enum<'_> {
    fn fmt(&self, name: Name, f: &mut impl std::io::Write) -> io::Result<()> {
        let max_name_len = self.max_name_len();
        let max_value_len = self.max_value_len();

        match name {
            Name::Full => {
                writeln!(f, "enum {}\n{{", self.name.to_string())?;
            }
            Name::RemoveNamespace(ns) => {
                writeln!(f, "enum {}\n{{", ns.strip(&self.name.to_string()))?;
            }
        }

        for value in &self.values {
            write!(f, "\t{}", value.name.to_string())?;
            formatter::pad_spaces_t(f, value.name.len(), max_name_len)?;

            #[expect(clippy::unnecessary_cast)]
            let value = match value.value {
                pdb::Variant::U8(v) => v as i64,
                pdb::Variant::U16(v) => v as i64,
                pdb::Variant::U32(v) => v as i64,
                pdb::Variant::U64(v) => v as i64,
                pdb::Variant::I8(v) => v as i64,
                pdb::Variant::I16(v) => v as i64,
                pdb::Variant::I32(v) => v as i64,
                pdb::Variant::I64(v) => v as i64,
            };

            write!(f, "\t= ")?;

            match (max_value_len, value >= 0) {
                (0, true) => write!(f, "0x{value:01x}")?,
                (1, true) => write!(f, "0x{value:02x}")?,
                (2, true) => write!(f, "0x{value:03x}")?,
                (_, true) => write!(f, "0x{value:04x}")?,
                (0, false) => write!(f, "-0x{value:01x}", value = value.abs())?,
                (1, false) => write!(f, "-0x{value:02x}", value = value.abs())?,
                (2, false) => write!(f, "-0x{value:03x}", value = value.abs())?,
                (_, false) => write!(f, "-0x{value:04x}", value = value.abs())?,
            }

            writeln!(f, ",")?;
        }

        writeln!(f, "}};")?;

        Ok(())
    }

    fn max_name_len(&self) -> usize {
        formatter::get_max_length(&self.values, |value| value.name.len())
    }

    #[expect(clippy::unnecessary_cast)]
    #[expect(clippy::cast_abs_to_unsigned)]
    fn max_value_len(&self) -> u32 {
        let mut max_value_len = 0;
        for value in &self.values {
            max_value_len = max_value_len.max(match value.value {
                pdb::Variant::U8(v) => v as u64,
                pdb::Variant::U16(v) => v as u64,
                pdb::Variant::U32(v) => v as u64,
                pdb::Variant::U64(v) => v as u64,
                pdb::Variant::I8(v) => v.abs() as u64,
                pdb::Variant::I16(v) => v.abs() as u64,
                pdb::Variant::I32(v) => v.abs() as u64,
                pdb::Variant::I64(v) => v.abs() as u64,
            });
        }
        max_value_len.max(1).ilog2() / 4
    }
}

impl ForwardReference {
    fn fmt(&self, f: &mut impl std::io::Write) -> io::Result<()> {
        writeln!(
            f,
            "{} {};",
            match self.kind {
                ForwardReferenceKind::Class => "class",
                ForwardReferenceKind::Struct => "struct",
                ForwardReferenceKind::Enum => "enum",
                ForwardReferenceKind::Unknown => "class",
                ForwardReferenceKind::Typedef => "typedef",
                ForwardReferenceKind::TypedefInner => "class",
            },
            self.name,
        )
    }
}

//
// fs helpers
//

enum HeaderType {
    Enum,
    Class,
}

fn create_header_file(
    resident_type_name: &pdb::RawString,
    namespace: &Namespace,
    header_type: HeaderType,
    output_path: &mut std::path::PathBuf,
    header_path: &mut std::path::PathBuf,
    flags: GenFlags,
    files: &mut Files,
) -> crate::Result<Option<std::fs::File>> {
    let Some(header_name) = build_header_name(
        resident_type_name,
        namespace,
        header_type,
        header_path,
        flags,
    ) else {
        return Ok(None);
    };

    header_path.push(format!("{header_name}.h"));

    utils_fs::open_file(output_path, header_path, files, ".h").map(Some)
}

fn build_header_name(
    resident_type_name: &pdb::RawString,
    namespace: &Namespace,
    header_type: HeaderType,
    header_path: &mut std::path::PathBuf,
    flags: GenFlags,
) -> Option<String> {
    const MAX_CLASS_LEN: usize = 140;

    let root_name = match namespace.get_root() {
        Some(root_name) => root_name,
        None if flags.contains(GenFlags::SKIP_NON_ENGINE_HEADERS) => return None,
        None => "others",
    };

    header_path.push(root_name);
    if let Some(class_name) = namespace.get_class() {
        header_path.push(class_name);
    }
    if let Some(subclass_name) = namespace.get_subclass() {
        header_path.push(subclass_name);
    }

    match header_type {
        HeaderType::Enum => header_path.push("enums"),
        HeaderType::Class => (),
    }

    let header_name = resident_type_name.to_string();
    let header_name = namespace.strip(&header_name);
    let header_name = &header_name[0..header_name.len().min(MAX_CLASS_LEN)];
    let header_name = header_name
        .replace(":", "_")
        .replace("*", "+")
        .replace("&", "+")
        .replace("<", "_")
        .replace(">", "_");
    Some(header_name)
}

fn starts_with_equal_group(lhs: &str, rhs: &str) -> bool {
    let mut result = false;

    for (i, (lhc, rhc)) in lhs.chars().zip(rhs.chars()).enumerate() {
        if lhc != rhc {
            break;
        }
        if lhc == '_' && rhc == '_' && i > 1 {
            result = true;
            break;
        }
    }

    result
}

fn ends_with_equal_group(lhs: &str, rhs: &str) -> bool {
    let mut result = false;

    for (i, (lhc, rhc)) in lhs.chars().rev().zip(rhs.chars().rev()).enumerate() {
        if lhc != rhc {
            break;
        }
        if lhc == '_' && rhc == '_' && i > 1 {
            result = true;
            break;
        }
    }

    result
}

impl ForwardReferenceKind {
    pub fn from_class_kind(kind: pdb::ClassKind) -> Self {
        match kind {
            pdb::ClassKind::Class => Self::Class,
            pdb::ClassKind::Struct => Self::Struct,
            pdb::ClassKind::Interface => unreachable!(),
        }
    }
}

fn find_forward_reference(ty: &str, this: &Type, ns: &Namespace) -> Option<ForwardReference> {
    let ty = skip_default_environment(ty)?;

    if !ty.contains("vostok") && !ty.contains("survarium") {
        return None;
    }

    let mut ty = ty.as_str();
    let mut kind = ForwardReferenceKind::Unknown;
    let mut usage = ForwardReferenceUsage::ByValue;

    if ty.ends_with('*') || ty.ends_with('&') {
        usage = ForwardReferenceUsage::ByReference;
        // sushi@NOTE: Not the best way to remove all possible `const *&` at the end
        ty = ty
            .trim_suffix("*")
            .trim_suffix("*")
            .trim_suffix("&")
            .trim_suffix("&")
            .trim_suffix("*")
            .trim_suffix("*")
            .trim_suffix("&")
            .trim_suffix("&")
            .trim_suffix("const")
            .trim_end()
    }

    if &Type::new(ty, ns) == this {
        return None;
    }

    if let Some(targ) = type_builder::extract_forward_reference_from_template(ty) {
        usage = ForwardReferenceUsage::ByValue;
        ty = targ;
    }

    let name = ty.to_string();

    if name.ends_with("_enum") {
        // sushi@TODO: We can actually do better here, because we are parsing enums also.
        // The only problem is that we doing it while building those structs.
        // To fix this we would need to split parsing into two stages or first parse, then
        // fix forward references and only then write to the filesystem
        kind = ForwardReferenceKind::Enum;
    }

    Some(ForwardReference { kind, usage, name })
}

fn skip_default_environment(ty: &str) -> Option<&str> {
    const DEFAULT_ENVIRONMENT: &[&str] = &[
        "boost::noncopyable",
        "vostok::mutable_buffer",
        "vostok::resources::managed_resource",
        "vostok::math::float2",
        "vostok::math::float3",
        "vostok::math::float4",
        "vostok::math::float4x4",
    ];

    for skipped_ty in DEFAULT_ENVIRONMENT {
        if ty.starts_with(skipped_ty) {
            return None;
        }
    }
    Some(ty)
}
