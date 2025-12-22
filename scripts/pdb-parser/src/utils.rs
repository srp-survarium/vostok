use pdb_addr2line::type_parser;
use pdb_addr2line::type_parser::AttributeFlags;
use pdb_addr2line::type_parser::ReturnType;

use std::borrow::Cow;

#[derive(Default, Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub struct Type(pub String);

impl std::fmt::Display for Type {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.0.fmt(f)
    }
}

impl Type {
    pub fn new(ty: &str, namespace: &Namespace) -> Self {
        let mut ty = Self::new_impl(ty);

        if let Some(ref raw_subclass) = namespace.raw_subclass {
            ty = ty.replace(raw_subclass, "");
        }

        if let Some(ref raw_class) = namespace.raw_class {
            ty = ty.replace(raw_class, "");
        }

        if let Some(ref raw_root) = namespace.raw_root {
            ty = ty.replace(raw_root, "");

            if raw_root == &"survarium::" {
                ty = ty.replace("vostok::", "");
            }
        }

        Self(ty)
    }

    pub fn new_forward_declare(ty: &str) -> Self {
        Self(Self::new_impl(ty))
    }

    fn new_impl(ty: &str) -> String {
        let ty = Cow::Borrowed(ty);

        let ty = {
            let resource_ptr = "vostok::resources::resource_ptr<";
            match replace_by_first_template_arg(&ty, resource_ptr, "_ptr") {
                None => ty,
                Some(ty) => Cow::Owned(ty),
            }
        };

        let ty = {
            let intrusive_ptr = "vostok::intrusive_ptr<";
            match replace_by_first_template_arg(&ty, intrusive_ptr, "_ptr") {
                None => ty,
                Some(ty) => Cow::Owned(ty),
            }
        };

        let ty = {
            let intrusive_list = "vostok::intrusive_list<";
            match replace_by_first_template_arg(&ty, intrusive_list, "_list") {
                None => ty,
                Some(ty) => Cow::Owned(ty),
            }
        };

        let ty = {
            let vector = "stlp_std::vector<";
            match extract_template_arg(&ty, vector) {
                None => ty,
                Some(ty) => Cow::Owned(format!("{vector}{ty} >")),
            }
        };

        #[rustfmt::skip]
        let ty = ty
            //
            // Generic type replacements
            //
            .replace("stlp_std",     "std")
            .replace("char const*",  "pcstr")
            .replace("char const *", "pcstr")
            .replace("void const*",  "pcvoid")
            .replace("void const *", "pcvoid")

            .replace("u8 const *", "pcbyte")
            .replace("u8 const*",  "pcbyte")
            .replace("u8 *",       "pbyte")
            .replace("u8*",        "pbyte")

            //
            .replace("unsigned int",       "u32")
            .replace("unsigned short",     "u16")
            .replace("unsigned char",      "u8")
            //
            .replace("boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> >",     "boost::asio::ip::tcp::socket")
            .replace("boost::asio::ip::basic_endpoint<boost::asio::ip::tcp>",                                                                "boost::asio::ip::tcp::endpoint")
            .replace("boost::asio::ip::basic_resolver<boost::asio::ip::tcp,boost::asio::ip::resolver_service<boost::asio::ip::tcp> >",       "boost::asio::ip::tcp::resolver")
            .replace("boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>",                                                       "boost::asio::ip::tcp::resolver::iterator")
            .replace("boost::asio::ip::basic_resolver_query<boost::asio::ip::tcp>",                                                          "boost::asio::ip::tcp::resolver::query")

            .replace("boost::asio::basic_datagram_socket<boost::asio::ip::udp,boost::asio::datagram_socket_service<boost::asio::ip::udp> >", "boost::asio::ip::udp::socket")
            .replace("boost::asio::ip::basic_endpoint<boost::asio::ip::udp>",                                                                "boost::asio::ip::udp::endpoint")


            .replace("boost::asio::basic_streambuf<std::allocator<char> >",                  "boost::asio::streambuf")
            .replace("boost::noncopyable_::noncopyable",                                     "boost::noncopyable")
            .replace("std::basic_string<char,std::char_traits<char>,std::allocator<char> >", "std::string")
            .replace("std::basic_ostream<char,std::char_traits<char> >",                     "std::ostream")
            .replace("std::basic_istream<char,std::char_traits<char> >",                     "std::istream")


            .replace(" __cdecl(void)", "()")
            .replace(" __cdecl", "")

            // See `extensions.h`. Used also in `DEFAULT_ENVIRONMENT`.
            .replace("vostok::math::float2",   "float2")
            .replace("vostok::math::float3",   "float3")
            .replace("vostok::math::float4",   "float4")
            .replace("vostok::math::float4x4", "float4x4")

            // Consistent formatting for templates and function signatures
            .replace(", ", ",")
            .replace(",", ", ")

            .replace("<", "< ")
            .replace(" >", ">")
            .replace(">", " >")

            .replace(" &", "&")
            .replace(" *", "*")

            .replace("(", "( ")
            .replace(")", " )")
            .replace("(  )", "()")
            ;

        ty
    }

    pub fn len(&self) -> usize {
        self.0.len()
    }
}

#[derive(Default, Clone, PartialEq)]
pub struct Namespace {
    raw_root: Option<&'static str>, // vostok::
    raw_class: Option<String>,      // vostok::network_core::
    raw_subclass: Option<String>,   // vostok::animation::mixing::
}

impl Namespace {
    pub fn get_from_class_name(fun: &type_parser::Function) -> Self {
        Self::get_from_class_name_impl(&fun.name)
    }

    pub fn get_from_class_name_impl(p: &str) -> Self {
        let mut iter = p.split("::").peekable();

        let root = iter.next();

        let class = {
            let class = iter.next();
            let class_is_last = iter.peek().is_none();
            match class {
                Some(class) if class_is_last || class.contains('<') => None,
                Some(class) => Some(class),
                None => None,
            }
        };

        // vostok::ai::planning::pddl_planner::forward_search_required
        // vostok::ai::perceptors::pickup_item_perceptor::`scalar deleting destructor'
        // vostok::ai::path_constructor::base::vertex_impl<vostok::sound::search::search_service::vertex_type>
        // vostok::ai::graph_wrapper::propositional_planner_base::impl
        // vostok::ai::sensors::vision_sensor::update_visibility_value
        // vostok::animation::mixing::addition_lexeme::addition_lexeme
        // vostok::core::configs::binary_config_cook::`scalar deleting destructor'
        // vostok::memory::detail::get_top_pointer_helper<boost::asio::ip::basic_resolver<boost::asio::ip::tcp,boost::asio::ip::resolver_service<boost::asio::ip::tcp> >,0>
        // vostok::render::culling::portal_sector_system::portal_sector_system
        // vostok::render::debug::renderer::draw_line_ellipsoid
        #[rustfmt::skip]
        let subclass = {
            let subclass = iter.next();
            let subclass_is_last = iter.peek().is_none();

            match (class, subclass) {
                (_, Some(_)) if subclass_is_last => None,
                (Some("ai"),        Some("planning"))         => subclass,
                (Some("ai"),        Some("perceptors"))       => subclass,
                (Some("ai"),        Some("path_constructor")) => subclass,
                (Some("ai"),        Some("graph_wrapper"))    => subclass,
                (Some("ai"),        Some("sensors"))          => subclass,
                (Some("animation"), Some("mixing"))           => subclass,
                (Some("core"),      Some("configs"))          => subclass,
                (Some("memory"),    Some("detai"))            => subclass,
                (Some("render"),    Some("culling"))          => subclass,
                (Some("render"),    Some("debug"))            => subclass,
                _ => None,
            }
        };

        match (root, class, subclass) {
            (Some("survarium"), _, _) => Self {
                raw_root: Some("survarium::"),
                raw_class: None,
                raw_subclass: None,
            },
            (Some("vostok"), None, _) => Self {
                raw_root: Some("vostok::"),
                raw_class: None,
                raw_subclass: None,
            },
            (Some("vostok"), Some(class), None) => Self {
                raw_root: Some("vostok::"),
                raw_class: Some(format!("vostok::{class}::")),
                raw_subclass: None,
            },
            (Some("vostok"), Some(class), Some(subclass)) => Self {
                raw_root: Some("vostok::"),
                raw_class: Some(format!("vostok::{class}::")),
                raw_subclass: Some(format!("vostok::{class}::{subclass}::")),
            },
            _ => Self {
                raw_root: None,
                raw_class: None,
                raw_subclass: None,
            },
        }
    }

    pub fn get_root(&self) -> Option<&'static str> {
        let root = self.raw_root?;
        let root = &root[0..root.len() - "::".len()];
        Some(root)
    }

    pub fn get_class(&self) -> Option<&str> {
        let root = self.raw_root?;
        let class = self.raw_class.as_ref()?;

        let class = &class[root.len()..class.len() - "::".len()];
        Some(class)
    }

    pub fn get_subclass(&self) -> Option<&str> {
        let class = self.raw_class.as_ref()?;
        let subclass = self.raw_subclass.as_ref()?;

        let subclass = &subclass[class.len()..subclass.len() - "::".len()];
        Some(subclass)
    }

    pub fn start_namespace(&self, w: &mut impl std::io::Write) -> crate::Result<()> {
        let mut new_line = false;
        if let Some(root) = self.get_root() {
            new_line = true;
            writeln!(w, "namespace {root} {{")?;
        }

        if let Some(class) = self.get_class() {
            new_line = true;
            writeln!(w, "namespace {class} {{")?;
        }

        if let Some(subclass) = self.get_subclass() {
            new_line = true;
            writeln!(w, "namespace {subclass} {{")?;
        }

        if new_line {
            writeln!(w)?;
        }

        Ok(())
    }

    pub fn end_namespace(&self, w: &mut impl std::io::Write) -> crate::Result<()> {
        if let Some(subclass) = self.get_subclass() {
            writeln!(w, "}} // namespace {subclass}")?;
        }
        if let Some(class) = self.get_class() {
            writeln!(w, "}} // namespace {class}")?;
        }
        if let Some(root) = self.get_root() {
            writeln!(w, "}} // namespace {root}")?;
        }

        Ok(())
    }

    pub fn strip<'a>(&self, class_name: &'a str) -> &'a str {
        if let Some(raw_subclass) = &self.raw_subclass {
            if let Some(class_name) = class_name.strip_prefix(raw_subclass) {
                return class_name;
            }
        }

        if let Some(raw_class) = &self.raw_class {
            if let Some(class_name) = class_name.strip_prefix(raw_class) {
                return class_name;
            }
        }

        if let Some(raw_root) = &self.raw_root {
            if let Some(class_name) = class_name.strip_prefix(raw_root) {
                return class_name;
            }
        }

        class_name
    }
}

//
//
//

pub fn write_fn_signature_with_args(
    fn_t: &type_parser::Function,
    namespace: &Namespace,
    args: &[(String, Type)],
    max_return_type_len: Option<usize>,
    max_method_name_len: Option<usize>,
    pad_args_len: Option<usize>,
    w: &mut impl std::io::Write,
) -> std::io::Result<()> {
    let type_parser::Function {
        return_type, name, ..
    } = fn_t;

    write_return_type(return_type, args.len(), namespace, max_return_type_len, w)?;

    let name = namespace.strip(name);
    write!(w, "{name}")?;

    if let Some(max_method_name_len) = max_method_name_len {
        pad_spaces_t(w, name.len(), max_method_name_len)?;
    }

    write!(w, "(")?;
    if args.len() < 4 {
        for (idx, (arg_name, arg_type)) in args.iter().enumerate() {
            let first = idx == 0;

            write!(w, "{n} {arg_type} ", n = if first { "" } else { "," })?;
            write!(w, "{arg_name}")?;
        }

        write!(w, " )")?;
    } else {
        writeln!(w)?;

        let len = args.len();

        let pad_space = args
            .iter()
            .map(|(_, arg_type)| arg_type.len())
            .max()
            .unwrap();

        for (idx, (arg_name, arg_type)) in args.iter().enumerate() {
            let last = idx == len - 1;

            if let Some(pad_args_len) = pad_args_len {
                pad_spaces_uncap(w, pad_args_len)?;
            }

            write!(w, "\t{arg_type}\t")?;

            pad_spaces_t(w, arg_type.len(), pad_space)?;
            match last {
                false => writeln!(w, "{arg_name},")?,
                true => writeln!(w, "{arg_name}")?,
            }

            if last {
                if let Some(pad_args_len) = pad_args_len {
                    pad_spaces_uncap(w, pad_args_len)?;
                }
                write!(w, ")")?;
            }
        }
    }

    if fn_t.attrs.contains(AttributeFlags::IS_CONST) {
        write!(w, " const")?;
    }

    Ok(())
}

pub fn write_fn_signature_unnamed_args(
    fn_t: &type_parser::Function,
    namespace: &Namespace,
    max_return_type_len: Option<usize>,
    max_method_name_len: Option<usize>,
    pad_args_len: Option<usize>,
    w: &mut impl std::io::Write,
) -> std::io::Result<()> {
    let args = fn_t
        .arg_types
        .iter()
        .enumerate()
        .map(|(i, arg_type)| (format!("arg_{i}"), Type::new(arg_type, namespace)))
        .collect::<Vec<_>>();

    write_fn_signature_with_args(
        fn_t,
        namespace,
        &args,
        max_return_type_len,
        max_method_name_len,
        pad_args_len,
        w,
    )
}

pub fn write_return_type(
    return_type: &type_parser::ReturnType,
    args_len: usize,
    namespace: &Namespace,
    max_return_type_len: Option<usize>,
    w: &mut impl std::io::Write,
) -> std::io::Result<()> {
    let return_type = get_return_type(return_type, args_len, namespace);
    write!(w, "{return_type}")?;

    if let Some(max_return_type_len) = max_return_type_len {
        write!(w, "\t")?;
        pad_spaces_t(w, return_type.len(), max_return_type_len)?;
    } else {
        write!(w, " ")?;
    }

    Ok(())
}

pub fn get_return_type(
    return_type: &type_parser::ReturnType,
    args_len: usize,
    namespace: &Namespace,
) -> std::borrow::Cow<'static, str> {
    match return_type {
        ReturnType::Constructor if args_len == 1 => std::borrow::Cow::Borrowed("explicit"),
        ReturnType::Constructor | ReturnType::Destructor => std::borrow::Cow::Borrowed(""),
        ReturnType::Type(type_) => std::borrow::Cow::Owned(Type::new(type_, namespace).0),
    }
}

pub fn set_method_attributes(
    fn_t: &mut type_parser::Function,
    attrs: pdb::FieldAttributes,
    found_body: bool,
) {
    let attrs = method_attributes::MyFieldAttributes::extract(attrs);

    #[rustfmt::skip]
    {
        fn_t.attrs.set(AttributeFlags::IS_VIRTUAL,  attrs.is_virtual());
        fn_t.attrs.set(AttributeFlags::IS_OVERRIDE, attrs.is_override());
        fn_t.attrs.set(AttributeFlags::IS_PURE,     attrs.is_pure());
        fn_t.attrs.set(AttributeFlags::IS_FINAL,    attrs.is_final());

        // Method is considered to be inline if it isn't pure, it's body wasn't found (meaning it
        // was most likely inlined) and that the body wasn't generated by compiler
        fn_t.attrs.set(
            AttributeFlags::IS_INLINE,
            fn_t.attrs.contains(AttributeFlags::IS_INLINE) || (!attrs.is_pure() && !found_body),
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

//
//
//

pub fn write_fmt(
    w: &mut impl std::io::Write,
    cb: impl Fn(&mut String) -> std::io::Result<()>,
) -> std::io::Result<()> {
    let mut result = String::new();
    cb(&mut result).unwrap();
    write!(w, "{result}")
}

/// Padding between a type and name. Used for arguments, constants & statics.
pub const MAX_PAD_TABS: usize = 9;
pub const MAX_PAD_SPACE: usize = MAX_PAD_TABS * 4;

/// Pad a string given how much was already written
///
/// Use this function if you don't care about the size of the total padding.
pub fn pad_spaces(w: &mut impl std::io::Write, prefix_len: usize) -> std::io::Result<()> {
    pad_spaces_t(w, prefix_len, MAX_PAD_SPACE)
}

/// Pad a string given how much was already written and how big the padding needs to be.
///
/// # Arguments
///
/// * `prefix_len` - How much bytes were already written
/// * `pad_space`  - Length of the paddding you want to achieve.
///   Note that it will be capped by `MAX_PAD_SPACE`.
pub fn pad_spaces_t(
    w: &mut impl std::io::Write,
    prefix_len: usize,
    pad_space: usize,
) -> std::io::Result<()> {
    for _ in 0..pad_times(prefix_len, pad_space.min(MAX_PAD_SPACE)) {
        write!(w, "\t")?;
    }
    Ok(())
}

/// Pad a string to `pad_space` length.
///
/// # Arguments
///
/// * `pad_space`  - Length of the paddding you want to achieve.
///   Note that it will be capped by `MAX_PAD_SPACE`.
pub fn pad_spaces_uncap(w: &mut impl std::io::Write, pad_space: usize) -> std::io::Result<()> {
    for _ in 0..pad_times(0, pad_space) {
        write!(w, "\t")?;
    }
    Ok(())
}

pub fn pad_times(prefix_len: usize, pad_space: usize) -> usize {
    //
    // my_type
    // <--><--><--><--><--><-
    //    ^                 ^
    //    already_tabbed    pad_space
    //     <--><--><--><--><-->
    //

    let pad_tabs = (pad_space % 4 != 0) as usize + pad_space / 4;

    let already_tabbed = prefix_len / 4;

    pad_tabs.saturating_sub(already_tabbed)
}

fn ignore_long_length(length: usize) -> usize {
    match length >= MAX_PAD_SPACE {
        true => 0,
        false => length,
    }
}

pub fn get_max_length<T>(items: &[T], f: impl Fn(&T) -> usize) -> usize {
    items
        .iter()
        .map(|item| ignore_long_length(f(item)))
        .max()
        .unwrap_or_default()
}

/// See `test_extract_template_arg`.
pub fn extract_template_arg<'a>(ty: &'a str, prefix: &str) -> Option<&'a str> {
    if !ty.starts_with(prefix) {
        return None;
    }

    let ty = &ty[prefix.len()..];
    let vector_end = find_impl(ty, ',');
    let ty = &ty[0..vector_end];

    Some(ty)
}

pub fn find_extract_template_arg<'a>(ty: &'a str, prefix: &str) -> Option<&'a str> {
    let pos = ty.find(prefix)?;

    let ty = &ty[pos + prefix.len()..];
    let vector_end = find_impl(ty, ',');
    let ty = &ty[0..vector_end];

    Some(ty)
}

/// See `test_replace_by_first_template_arg`.
pub fn replace_by_first_template_arg(
    ty: &str,
    template_prefix: &str,
    res_ty_suffix: &str,
) -> Option<String> {
    let pos_start = ty.find(template_prefix)?;

    let (res_prefix, buffer) = ty.split_at(pos_start);
    let res_ty = extract_template_arg(buffer, template_prefix)?;

    let buffer_suffix = &buffer[template_prefix.len() + res_ty.len()..];
    let pos_end = find_impl(buffer_suffix, '>') + 1;
    let res_suffix = &buffer_suffix[pos_end..];

    Some(format!("{res_prefix}{res_ty}{res_ty_suffix}{res_suffix}"))
}

fn find_impl(ty: &str, sep: char) -> usize {
    let mut depth = 0;
    let mut pos = 0;
    for (i, c) in ty.chars().enumerate() {
        match c {
            _ if depth == 0 && c == sep => {
                pos = i;
                break;
            }
            '<' => depth += 1,
            '>' => depth -= 1,
            _ => (),
        }
    }
    pos
}

#[test]
pub fn test_extract_template_arg() {
    let input = "std::vector<collision_geometry_subscriber*, std::allocator< collision_geometry_subscriber* > >";
    let dejure = "collision_geometry_subscriber*";
    let defacto = extract_template_arg(input, "std::vector<").unwrap();
    assert_eq!(dejure, defacto);

    let input = "std::vector<collision_geometry_subscriber<2, 3>*, std::allocator< collision_geometry_subscriber* > >";
    let dejure = "collision_geometry_subscriber<2, 3>*";
    let defacto = extract_template_arg(input, "std::vector<").unwrap();
    assert_eq!(dejure, defacto);
}

#[test]
pub fn test_replace_by_first_template_arg() {
    let input = "boost::array<vostok::intrusive_list<survarium::affect_subscriber, affect_subscriber*, 32, threading::mutex, size_policy, no_debug_policy >, 9 >";
    let dejure = "boost::array<survarium::affect_subscriber_list, 9 >";
    let defacto = replace_by_first_template_arg(input, "vostok::intrusive_list<", "_list").unwrap();
    assert_eq!(dejure, defacto);
}
