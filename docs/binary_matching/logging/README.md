# logging - matching notes

- Namespace: `vostok::logging`.
- Build: `/Od` (optimization off) + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/logging/sources/` (low-level); high-level API in `vostok/core/logging_extensions`.
- Reachability anchor: `use_log` in `game/sources/temp_include_all.cpp`
  (lived in game_core until the game carcass rebuild).
- Status: reconstructed; remaining emitted-code gaps are audited below.

### Matching dept

* Previously there were multiple `operator()` overloads, which allowed `LOG_*` macro to pass flags and log location. With current `append` functions this was not possible and is instead replaced with a menagerie of `LOG_*` macros: `LOG_ERROR`, `LOGF_ERROR`, `LOGFD_ERROR`, `LOGIFD_ERROR` `LOGFD_FORCED`, etc. I don't think this is how they actually wrote this code, but based on the pdb file I cannot assume anything else for now.
* Also those macro are not compiled into the same assembly. This needs to be investigated further.
* Devs forgot to remove `static vostok::logging::log_callback_boost	s_log_callback` in `logger.cpp`. It doesn't seem to be used by anything according to IDA, but it did generate a dynamic constructor function, which is not generated in my case. Why? Maybe I should initialize it to something?
  * claude (2026-06): the base emits both helpers. Their PDBs use different demangled spellings;
    `match_db.py` safely canonicalizes plain fully-qualified static names before pairing. The six
    `format_*` atexit helpers remain a compiler-context wall: the target calls the folded empty
    destructor while the base elides it. Explicitly declaring the empty destructor leaves the base
    thunks unchanged.
* `verbosity_to_string` preserves the shipped error/info display-string swap. Target and base have
  the same seven symbolic returns and statement structure; the remaining report residual is the
  comparator's failed absolute-relocation handling for the embedded switch jump table.

#### LOG_* macro verdict (claude, 2026-06, from target bytes at a real call site)

Probed `survarium::weapon_core_fire_state_base::on_shot_event` (game_core, /Od) - a `LOG_ERROR`
site - target vs base, instruction level. Result: **the `__LOG` macro reconstruction is
byte-faithful**; the "operator() overloads" theory is NOT what the bytes show. The target emits
exactly the macro shape:

1. `cmp g_log_filter_tree,0 / je <log>` then `push verbosity; push "module:"; push tree;
   call logging::has_passed_filters` + `jne <log> / jmp <skip>` - the `__CHECK_VERBOSITY( ) ? : NULL`
   ternary, byte-identical in base.
2. the log arm constructs a `log_callback_boost` temp from `g_log_callback` (LTCG-cloned boost
   `function<>` ctor - the fn-ptr arg is folded INTO the clone, no visible arg), then pushes,
   right-to-left: format-string literal, verbosity imm, `"game_core:"` initiator literal,
   `__FUNCSIG__` literal, `__LINE__` imm, `__FILE__` literal, `&g_log_format`, `g_log_flags`,
   `&temp`, and `call vostok::logging::append` (cdecl, varargs).

Identification rules for a diverging LOG site (read the target pushes):
* no `has_passed_filters` guard at all -> `*_FORCED` variant (`__LOG_FORCED`).
* which `append` overload is called: `log_format const*` arg (`push ?g_log_format@core@...`)
  -> plain `LOG_*` / `LOGI_*`; `format_specifier const&` arg (push of `format_message` /
  `format_time...` / a local `format_specifier`) -> `LOGF_*` / `LOGFD_*` / `LOGIFD_*`.
* user-data push: `g_log_flags` -> non-`D` variant; anything else (a `core::log_to_console`
  constant, an object ptr) -> `D` variant (`LOGFD`/`LOGIFD`).
* initiator literal: `"module:"` -> no `I`; `"module:sub:"` -> `I` variant (the macro
  string-concatenates, so the sub-initiator is INSIDE the same literal).
* verbosity immediate picks ERROR/WARNING/INFO/TRACE/DEBUG.

Residuals at a correct site (bank, do not chase):
* ~3 bytes from the `log_callback_boost` temp ctor: LTCG schedules the cloned ctor BEFORE the
  arg pushes in the target (custom conv, `this` in eax) but after them in base (`this` in esi) -
  argument-boundary LTCG.
* `__LINE__` immediate differs when our line number differs from the original's. WATCH: lines
  <= 127 push imm8 (2 bytes), >= 128 push imm32 (5 bytes) - if our site and the original's
  straddle that boundary the statement SIZE shifts by 3; keep log-bearing lines on the same side.

#### logging_extensions.cpp (core, /Ox) - swept 2026-06, 56.7 -> 69.4

* **`debug::log_callback` was a RAW function pointer, not boost::function.** The target PDB types
  `debug::set_log_callback` / `get_log_callback` as `void (*)(pcstr,bool,bool,pcstr) (...)`, and the
  target debug.cpp emits no dynamic initializer for its `s_log_callback`. Fixing the typedef in
  `vostok/debug/log_callback.h` (+ by-value param) took set/get to 100%, logging_finalize 63.7->100,
  logging_preinitialize 0->100, bugtrap::finalize and all `load_function<>` thunks in debug to 100%.
* **`debug_log_callback` calls `logging::append` DIRECTLY** (like `use_log`) - twice, NO
  `has_passed_filters` guard (the FORCED form), runtime-joined initiator (`STR_JOINA(debug_log,
  initiator, ":")` - no macro can pass a runtime initiator), verbosity `is_error ? error : info`,
  format `format_message` (format_specifier overload) in the `log_only_user_string` arm vs
  `&g_log_format` (log_format* overload) in the other, and user_data = **the ADDRESS of the local
  `log_flags`** (lea, a real target oddity reproduced as `(void*)&log_flags`). The not-set arm of
  log_flags is **0**, not log_to_console (target: `dec;neg;sbb;and 2` -> {0,2}).
* **`log_flags_enum` is a bitmask at the consumer**: `logging_callback` tests `user_data & 1` /
  `(user_data >> 1) & 1` - not `==` comparisons - and gates the stderr write on the stderr bit
  being SET inside an outer `g_log_filter_tree` check (with a redundant re-check around the stdout
  arm, original).
* `use_console_for_logging`: the static-init expression evaluates the KEY first:
  `s_use_console || testing::run_tests_command_line()` (46->98). Keep the operator-bool spelling -
  explicit `.is_set()` inlines a different guard shape and drops it back to 50.
* `generate_log_file_name` scored None purely on the MANGLED NAME: the target params are
  `native_path_string* const` / `pcstr const` (QAV/QBD) - adding the top-level consts paired the
  357-byte function (-> 55.6%, rest is /Ox LTCG inlining of user_data_directory + the convert loop).
* Remaining sub-100 here is /Ox+LTCG inlining (convert loops, key::is_set guards, boost function1
  ctor/dtor machinery) - link-set dependent, banked per function.

#### The module-wide systemic residual: core/boost helper inline-vs-call

Nearly every sub-100% logging function diverges for ONE reason: a tiny shared helper that the
TARGET calls out-of-line (usually with an LTCG custom convention / dropped args, ICF-folded with
other empties) while OUR base build inline-expands it - or vice versa. Confirmed instances:
`strings::compare_insensitive` (string_to_verbosity, flush), `vostok::vsnprintf`
(logger::operator()), `strings::copy<512>` (fill_log_string, execute, fill_specifier_list),
`buffer_string::find` + `fixed_string::c_str` (filter_is_overwritten), `fixed_string` `operator<`
(compare_nodes(nb,nb) - direction reversed: target inlines, base calls the unmarked template),
`threading::interlocked_exchange_pointer` (close, log_file ctor), `math::min`
(process_next_line x2), `memory::strip_pointer` (every VOSTOK_NEW/DELETE_IMPL site - it is an
identity fn, compiles to empty, folds with the assert stubs), boost intrusive hook ctor /
`init_header` / `~set_member_hook` / `rbtree_impl::find|end` (node ctor/dtor/set/get_verbosity),
`log_format::log_format()` + `logging::noncopyable` ctor/dtor (logger ctor expansion in append x2;
the target `format.h` unit's lone 1-byte `empty_stub` IS the un-expanded `log_format(){}` COMDAT).
These are link-set-dependent LTCG inline decisions (call-site counts differ between the full
original link and our partial one) and/or core-header reconstruction differences (the original
likely defined several of these out-of-line in core, e.g. `compare_insensitive`, `vsnprintf`,
`buffer_string::find`). FIXING THEM MEANS TOUCHING vostok/core / shared headers / boost - flagged,
NOT edited from logging (sushi rule: no cross-unit out-lining). Do not re-derive this per function;
the per-function `// VERDICT:` lines name the helper.

### Open questions

* There are `debug_log` functions, which it seems like I didn't use at all.
  * claude (2026-06): the only target symbol matching is `vostok::core::debug_log_callback`
    (core/logging_extensions.cpp - out of logging's scope). Nothing in vostok/logging itself.
* `log_file::on_terminate` is declared in `log_file.h` (so it was in the original class - the PDB
  type record carries it) but the target emits no body for it anywhere. Keep the declaration; an
  unverifiable out-of-line `{ close(); }` reconstruction was removed.
* `log_file::flush`: the target re-checks `strings::compare_insensitive( m_file_name.c_str(),
  in_file_name )` and returns early when the requested name equals the current log name -
  recovered 2026-06 (was a missing condition, not LTCG).

### Overview

The logging module is split into two parts.
* High level logic used by `LOG_*` macro is in `vostok/core/logging_extensions`.
* Low level logic used to build logs is in `vostok/logging`.

#### High level

* `vostok/core/logging_extensions` exports functions for initializing logging and global variables used by logging `LOG_` macro. Those global variables are stored in a global `pch.h`, which makes logging functions immediately available for the whole project:
```cpp
// in :/vostok/core/extensions.h
#include <vostok/logging/extensions.h>
#include <vostok/core/logging_extensions.h>
```

#### Low level

* `vostok/logging/path_parts.cpp`
    - Why does it keep index internally (both `m_index` and `m_current_element`).
    - What are actual values for different path parts?

* `vostok/logging/logger.cpp`
* `vostok/logging/filter_tree.cpp`
* `vostok/logging/filter_tree_node.cpp`
* `vostok/logging/format_specifier.cpp`
* `vostok/logging/logging_filters_console_command.cpp`
