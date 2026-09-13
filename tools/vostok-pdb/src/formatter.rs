use pdb_addr2line::type_parser;
use pdb_addr2line::type_parser::AttributeFlags;
use pdb_addr2line::type_parser::ReturnType;

use crate::{Namespace, Type};

/// Padding between a type and name. Used for arguments, constants & statics.
pub const MAX_PAD_TABS: usize = 9;
pub const MAX_PAD_SPACE: usize = MAX_PAD_TABS * 4;

pub enum Formatter {
    /// void on_execute(
    ///     u32         time_elapsed_ms,
    ///     bool        first_person
    /// )
    ///
    /// void on_remove( u32 time_elapsed_ms, bool first_person )
    Source,

    /// ```cpp
    /// class Test {
    ///     void     on_execute     (
    ///                 u32         time_elapsed_ms,
    ///                 bool        first_person,
    ///              )
    ///
    ///     void     on_remove     (
    ///                 u32         time_elapsed_ms,
    ///                 bool        first_person,
    ///              )
    ///
    ///     x------x                 <- max_return_type_len
    ///              x------------x  <- max_method_name_len
    ///     void     on_add        (
    /// x-----------x                <- pad_args_len
    ///                 u32         time_elapsed_ms,
    ///                 bool        first_person,
    ///              )
    ///
    /// x--x                         <- expected to be written
    ///     void     on_add        (
    ///                 u32         time_elapsed_ms,
    ///                    x-------x <- figured out inside the function
    ///                 bool        first_person,
    ///              );
    ///               ^              <- needs to be written afterwards
    /// }
    /// ```
    Header(HeaderFormatter),
}

pub struct HeaderFormatter {
    /// Padding in spaces after return type is written.
    pub max_return_type_len: usize,
    /// Padding in spaces after method name is written.
    pub max_method_name_len: usize,
    /// Padding in spaces after argument name is written.
    pub pad_args_len: usize,
}

struct FormatterInner {
    max_return_type_len: Option<usize>,
    max_method_name_len: Option<usize>,
    pad_args_len: Option<usize>,
    in_header: bool,
}

impl Formatter {
    pub fn write_fn_signature_with_args(
        self,
        fn_t: &type_parser::Function,
        namespace: &Namespace,
        margs: &[(String, Type)],
        w: &mut impl std::io::Write,
    ) -> std::io::Result<()> {
        let FormatterInner {
            max_return_type_len,
            max_method_name_len,
            pad_args_len,
            in_header,
        } = self.get_inner();

        let type_parser::Function {
            return_type, name, ..
        } = fn_t;

        // sushi@TODO: Should be in a different place, since this is still "parsing"
        let mut should_split = false;
        let mut total_length = 0;
        let args = {
            let mut args = vec![];
            for i in 0..fn_t.arg_types.len() {
                let arg_type = Type::new(&fn_t.arg_types[i], namespace);

                match margs.get(i) {
                    None => args.push((format!("arg_{i}"), arg_type)),
                    // Sometimes in headers arguments are defined as non-const,
                    // while in sources they are const by value
                    Some((marg_name, marg_type))
                        if marg_type == &arg_type
                            || marg_type.0.trim_prefix("const ").trim_suffix(" const")
                                == &arg_type.0 =>
                    {
                        args.push((
                            marg_name.clone(),
                            if in_header {
                                arg_type
                            } else {
                                marg_type.clone()
                            },
                        ));
                    }
                    // We didn't get the right type, the name is incorrect
                    Some((marg_name, marg_type)) => {
                        should_split = true;
                        args.push((format!("arg_{i} /* {marg_type} {marg_name} */"), arg_type));
                    }
                }
                total_length += args[i].0.len() + args[i].1.len() + 2;
            }
            args
        };
        should_split |= total_length > 80;
        should_split |= args.len() >= 4;

        write_return_type(return_type, args.len(), namespace, max_return_type_len, w)?;

        let name = namespace.strip(name);
        write!(w, "{name}")?;

        if let Some(max_method_name_len) = max_method_name_len {
            pad_spaces_t(w, name.len(), max_method_name_len)?;
        }

        write!(w, "(")?;
        if !should_split {
            for (idx, (arg_name, arg_type)) in args.iter().enumerate() {
                let first = idx == 0;

                write!(w, "{n} {arg_type} ", n = if first { "" } else { "," })?;
                write!(w, "{arg_name}")?;
            }

            write!(w, " )")?;
        } else {
            writeln!(w)?;

            let pad_space = get_max_length(&args, |(_, arg_type)| arg_type.len());

            for (idx, (arg_name, arg_type)) in args.iter().enumerate() {
                let last = idx == args.len() - 1;

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

    fn get_inner(self) -> FormatterInner {
        match self {
            Self::Source => FormatterInner {
                max_return_type_len: None,
                max_method_name_len: None,
                pad_args_len: None,
                in_header: false,
            },
            Self::Header(HeaderFormatter {
                max_return_type_len,
                max_method_name_len,
                pad_args_len,
            }) => FormatterInner {
                max_return_type_len: Some(max_return_type_len),
                max_method_name_len: Some(max_method_name_len),
                pad_args_len: Some(pad_args_len),
                in_header: true,
            },
        }
    }
}

impl HeaderFormatter {
    pub fn write_fn_signature_unnamed_args(
        self,
        fn_t: &type_parser::Function,
        namespace: &Namespace,
        w: &mut impl std::io::Write,
    ) -> std::io::Result<()> {
        let args = fn_t
            .arg_types
            .iter()
            .enumerate()
            .map(|(i, arg_type)| (format!("arg_{i}"), Type::new(arg_type, namespace)))
            .collect::<Vec<_>>();

        Formatter::Header(self).write_fn_signature_with_args(fn_t, namespace, &args, w)
    }
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

//
// Helpers
//

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
