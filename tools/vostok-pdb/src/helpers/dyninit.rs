//! Canonicalize compiler-generated static-initialization thunk symbols.
//!
//! MSVC emits a `??__E<var>` (dynamic initializer) and `??__F<var>` (dynamic
//! atexit destructor) thunk for every namespace/static-storage object with a
//! non-trivial ctor/dtor. The two PDBs we diff label these *differently*:
//!
//! * the original-game (target) PDB stores the **demangled**
//!   `` `dynamic initializer for 'X'' `` form (what `UnDecorateSymbolName` /
//!   DIA produces);
//! * our freshly built (base) PDB stores the **raw mangled** `??__E.../??__F...`
//!   form in the module Procedure symbol.
//!
//! Neither side carries a Public symbol for these thunks, so the rich index and
//! the delinker both fall back to the Procedure name — mangled on base,
//! demangled on target — and objdiff then fails to pair the *same* thunk.
//!
//! Canonicalizing the base side to the target's demangled form makes objdiff
//! pair them (and earn a real fuzzy/exact %). msvc-demangler's
//! `::`dynamic initializer'` rendering differs from the target's
//! `` `dynamic initializer for 'X'' `` style, so we reshape it here.

/// If `sym` is a `??__E` / `??__F` static-init thunk, return its demangled name
/// in the exact form the target PDB stores
/// (`` `dynamic initializer for 'X'' `` / `` `dynamic atexit destructor for 'X'' ``).
/// Returns `None` for any non-thunk symbol (leave it untouched).
pub fn canonicalize_static_init_thunk(sym: &str) -> Option<String> {
    let (kind, rest) = if let Some(r) = sym.strip_prefix("??__E") {
        ("dynamic initializer for", r)
    } else if let Some(r) = sym.strip_prefix("??__F") {
        ("dynamic atexit destructor for", r)
    } else {
        return None;
    };

    let inner = if rest.starts_with('?') {
        // Member / templated form: `rest` is a complete mangled DATA symbol
        // (e.g. `?Format@Image9GridVertex@Render@Scaleform@@2U..@A`) followed by
        // the thunk's `@@YAXXZ` function suffix. Demangling the *whole* `??__E?…`
        // thunk trips msvc-demangler ("bad number"), so demangle the inner data
        // symbol directly — its NAME_ONLY form is exactly the variable name the
        // target quotes.
        let data_sym = rest.strip_suffix("@@YAXXZ").unwrap_or(rest);
        demangle_name_only(data_sym)?
    } else {
        // Simple / namespaced form (`??__E<var>@<ns>@@YAXXZ`): the whole thunk
        // demangles to `<scope>::`dynamic initializer'`; the scope is the
        // fully-qualified variable name.
        let dm = demangle_name_only(sym)?;
        dm.strip_suffix("::`dynamic initializer'")
            .or_else(|| dm.strip_suffix("::`dynamic atexit destructor'"))?
            .to_string()
    };

    Some(format!("`{kind} '{inner}''"))
}

fn demangle_name_only(sym: &str) -> Option<String> {
    // NAME_ONLY drops the return type / calling convention; NO_CLASS_TYPE drops
    // the `class`/`struct` keyword inside template arguments — both match the
    // UnDecorateSymbolName form the target PDB stores
    // (e.g. `tree_space_param<2,vostok::rtp::grasping_tree_space_params>`, not
    // `<2,class vostok::rtp::grasping_tree_space_params>`).
    let flags =
        msvc_demangler::DemangleFlags::NAME_ONLY | msvc_demangler::DemangleFlags::NO_CLASS_TYPE;
    msvc_demangler::demangle(sym, flags).ok()
}

#[cfg(test)]
mod tests {
    use super::canonicalize_static_init_thunk as c;

    #[test]
    fn simple() {
        assert_eq!(
            c("??__Es_application@@YAXXZ").unwrap(),
            "`dynamic initializer for 's_application''"
        );
    }

    #[test]
    fn namespaced() {
        assert_eq!(
            c("??__Eg_allocator@engine@vostok@@YAXXZ").unwrap(),
            "`dynamic initializer for 'vostok::engine::g_allocator''"
        );
    }

    #[test]
    fn atexit_destructor() {
        assert_eq!(
            c("??__Fs_world@@YAXXZ").unwrap(),
            "`dynamic atexit destructor for 's_world''"
        );
    }

    #[test]
    fn templated_static_member() {
        // The `??__E?<member>@<templated-class>` form msvc-demangler can't parse
        // as a whole thunk, but the inner data symbol demangles cleanly.
        assert_eq!(
            c("??__E?Format@Image9GridVertex@Render@Scaleform@@2UVertexFormat@23@A@@YAXXZ")
                .unwrap(),
            "`dynamic initializer for 'Scaleform::Render::Image9GridVertex::Format''"
        );
    }

    #[test]
    fn not_a_thunk() {
        assert!(c("??0foo@@QAE@XZ").is_none());
        assert!(c("__FindPESection").is_none());
    }
}
