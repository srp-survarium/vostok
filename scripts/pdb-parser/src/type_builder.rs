use pdb_addr2line::type_parser;
use std::borrow::Cow;

#[derive(Default, Clone, PartialEq, Debug)]
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

    pub fn depth(&self) -> u32 {
        self.raw_root.is_some() as u32
            + self.raw_class.is_some() as u32
            + self.raw_subclass.is_some() as u32
    }
}

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
        let ty = match typedef_template(ty) {
            None => Cow::Borrowed(ty),
            Some(ty) => Cow::Owned(ty),
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

//
// Template extractors
//

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

pub fn extract_forward_reference_from_template(mut name: &str) -> Option<&str> {
    let mut found = false;
    for prefix in [
        "vostok::resources::resource_ptr<",
        "vostok::intrusive_ptr<",
        "vostok::intrusive_list<",
        "stlp_std::vector<",
    ] {
        let Some(targ) = find_extract_template_arg(name, prefix) else {
            continue;
        };
        found = true;
        name = targ
    }

    match found {
        true => Some(name),
        false => None,
    }
}

pub fn typedef_template(ty: &str) -> Option<String> {
    let mut ty = Cow::Borrowed(ty);
    for (prefix, suffix) in [
        ("vostok::resources::resource_ptr<", "_ptr"),
        ("vostok::intrusive_ptr<", "_ptr"),
        ("vostok::intrusive_list<", "_list"),
    ] {
        let Some(targ) = replace_by_first_template_arg(&ty, prefix, suffix) else {
            continue;
        };
        ty = Cow::Owned(targ);
    }

    let prefix = "stlp_std::vector<";
    if let Some(targ) = extract_template_arg(&ty, prefix) {
        ty = Cow::Owned(format!("{prefix}{targ} >"));
    }

    match ty {
        Cow::Owned(ty) => Some(ty),
        Cow::Borrowed(_) => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

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
        let defacto =
            replace_by_first_template_arg(input, "vostok::intrusive_list<", "_list").unwrap();
        assert_eq!(dejure, defacto);
    }
}
