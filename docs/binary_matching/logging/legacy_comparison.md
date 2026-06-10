# logging - legacy (xray-era) comparison

What the pre-rename history says about the open questions in [README.md](README.md).
Audited 2026-06 on `audit/logging-legacy-comparison` (tip `cffe77cd`).

## The rename

* `72175f57` "Rename engine to vostok" (2025-09-14, parent `184b4cd4`) - the rename on THIS
  branch's history. **Purely mechanical**: `git show 72175f57^:sources/xray/<f> | sed s/xray/vostok/...`
  diffs empty against the post-rename files for every logging file checked (logging.cpp,
  path_parts.cpp, format_specifier.cpp, extensions.h, format.h). The 53-97% git similarity
  scores are just `xray` token density, not edits.
* `39fb6bb1` "Rename engine to vostok (#2)" (2025-08-22) - the same mechanical rename on the
  separate `xray-2.0-dsgn-v0.100b` line; not an ancestor of this branch.

So "legacy" = anything at `72175f57^` and earlier, back to the initial import (`19dc1551`,
2025-08-05). That import is real GSC X-Ray 2.0 source (file headers 2008-2011, authors
Sergey Chechin / Dmitriy Iassenev) - **original-dev code**, but 3+ years older than the
v0.100b target, so it confirms intent/naming, not v0.100b bytes.

Two legacy generations coexist in the import:

1. **oldest, core-embedded** (`sources/xray/core/sources/logging*.cpp`, `xray/logging_extensions.h`):
   fastdelegate-based `log_callback`, `push_rule`/`pop_rule`, "rule_tree" naming.
2. **2011 standalone library** (`sources/xray/logging/`): boost::function callback,
   `filter_tree`, `push_filter` - the direct ancestor of today's `vostok/logging`.

## Per-component findings

### LOG_* macro layer - the operator() overloads DID exist (and were replaced)

Legacy `xray/logging/macros.h` + `helper.h`/`helper.cpp`:

```cpp
#define LOG_ERROR( ... )  ::xray::logging::check_verbosity(XRAY_LOG_MODULE_INITIATOR ":", error) ? \
        ::xray::logging::helper( __FILE__, __FUNCSIG__, __LINE__, XRAY_LOG_MODULE_INITIATOR ":", error )( ##__VA_ARGS__ ) : 0
```

`helper` is a temp object with FIVE `__cdecl operator()` overloads - `(pcstr,...)`,
`(log_flags_enum,pcstr,...)`, `(format_specifier const&,pcstr,...)`,
`(format_specifier const&,int,pcstr,...)`, `(log_format*,int,pcstr,...)` - all forwarding to a
static `process()` (vsnprintf into `string4096`, then `strings::iterate_items(..., '\n')` with a
`predicate` functor per line).

Verdict vs the sweep: **both READMEs are right - it's an evolution, not a contradiction.**
The operator() layer is 2011-real; by v0.100b the helper temp was replaced by free `append()`
overloads carrying the extra args (callback temp, user_data, format, initiator, verbosity) and
the guard became `g_log_filter_tree && has_passed_filters(...)`. The byte-verified `__LOG`
ternary IS the direct descendant of `check_verbosity(...) ? helper(...)(...) : 0` - same
guarded-ternary discipline, new callee. The overload-dispatch axes of the legacy operator()s
(no-extra / +flags / +format_specifier / +format_specifier+flags / +log_format*+flags) map
1:1 onto today's `LOG_* / LOGF_* / LOGFD_*` macro menagerie - so the macro variants are the
mechanical unrolling of the lost overloads, which softens the `sushi@NOTE` at `macros.h:58`:
the *shape* is faithful even if the original spelling possibly kept fewer macro names.
`LOG_FORCED`/`LOGI_FORCED` (no guard) already existed verbatim in 2011.

### path_parts - open question ANSWERED

Legacy `path_parts.h/.cpp` are byte-identical (modulo namespace) to our current ctor /
`add_part` / `to_next_element` - history confirms those three as original code.

* **Why both `m_index` and `m_current_element`:** two different cursors over a two-level
  structure. `m_parts` is `fixed_vector<pcstr,4>` of whole *part strings*; `m_index` is the
  index of the current part, `m_current_element` is a char cursor *inside* the current part
  (each part can carry several `:`-separated elements, because the macros concatenate
  `"module:" initiator ":"` into a single literal). `to_next_element` first `strchr`s for `:`
  within the part, only falling to `m_parts[++m_index]` at part end.
* **Actual values:** ctor pushes the initiator literal (unless it starts with `:`), then a
  NULL sentinel part. Typical content: `{ "game_core:sub:", NULL }`. `add_part` exists for
  multi-part paths but nothing in the macro path uses it beyond the ctor.
* `concat2buffer` is the one function that genuinely changed: legacy appends parts into a
  `buffer_string` via `operator+=` and trims a trailing `:` with `set_length`; v0.100b
  (matched 99.97%) does the raw `char[512]` + `strings::length` + `memory::copy` loop. Real
  rewrite by the original devs, not our invention.

### format / format_specifier - tree shape confirmed; v0.100b additions identified

* The expression-tree (`m_left`/`m_right`/`m_specifier`, `operator+`, recursive
  `fill_specifier_list` emitting `"%s"` per leaf and pushing enums; `format_separator`
  subclass with `fixed_string<128>`) is verbatim 2011 code. Our reconstruction is the
  original design.
* v0.100b deltas (all already in our tree): `format_specifier_time_brief` added (enum 7->8,
  global count 5->6), `format_string_type` changed `fixed_string<512>` -> `char[512]`, and the
  string ops in `fill_specifier_list` changed from fixed_string `=`/`+=` to
  `strings::copy`/`strings::append` free calls - which is exactly where the remaining 88%
  inline-vs-call residual sits. No hidden logic to adopt; the unit's 2/15 stays a
  `??__E`/`??__F` pairing artifact.
* `log_format::set` semantics confirmed by legacy: `indexes[i]` = specifier enum of the i-th
  `%s` in the format string, `enabled[spec]` = presence flag (drives `fill_log_string`'s six
  positional `c_str()` args).

### filter tree - node shapes confirmed verbatim

Legacy `filter_tree_node*.{h,cpp}` match our current shapes field-for-field:
`node_base{ set_member_hook tree_hook; fixed_string<32> name; }`,
`node : node_base { nodes_tree_type m_children; int m_verbosity; u32 m_thread_id; }`,
intrusive multiset with `compare<compare_nodes>` + `constant_time_size<false>` +
`auto_unlink` hooks. Also verbatim-confirmed: `recurse_0 = 256` OR'd into verbosity,
`node::set`'s find-or-new-child recursion, `get_verbosity`'s thread-id gate
(`m_thread_id != u32(-1) && != current_thread_id() -> silent`) + verbosity inheritance,
`compare_parts` treating `:` as a terminator, and `compare_nodes(nb,nb)` as
`left.name < right.name` (the fixed_string `operator<` wall is real original code).

Structural evolution (already reflected in our tree): legacy used a `globals_struct`
singleton (`rules_lock`, `root_rule` uninitialized_reference, `initiator_tree`,
`tree_allocator`, `filter_stack`); v0.100b packed it into the `filter_tree` class (0x38).
Legacy `get_tree_verbosity(path) -> int` became `has_passed_filters(initiator,verbosity) ->
bool` (matched 100%). Legacy `rule_is_overwritten` uses the same
`initiator.find(other.c_str()) == 0` idiom as our `filter_is_overwritten` - the 31% there is
purely the `buffer_string::find`/`c_str` call wall, shape is original. Legacy
`string_to_value` walked a NULL-terminated `verbosity_to_str` table with **case-sensitive**
`strings::compare`; the target's `compare_insensitive` is a real v0.100b change (matches the
sweep). Legacy `cc_logging_rule::execute/save_to` are the ancestors of
`logging_filters_console_command.cpp` (same get_token / single-arg-means-verbosity logic).

### log_file

* **`on_terminate` body recovered:** legacy defines `void log_file::on_terminate() { close(); }` -
  literally our "invented" STUB body. Reclassify: history-confirmed original code that LTCG
  strips from the v0.100b link (no live callers). Keep it.
* **`flush` early-return is genuinely new:** legacy `flush(in_file_name)` has NO
  name-equality check - the target's `compare_insensitive(m_file_name.c_str(), in_file_name)`
  early-out (recovered 2026-06) is a v0.100b addition, correctly recovered.
* The rest is verbatim lineage: `close()` = interlocked_exchange_pointer + full memory
  barrier + flush + close; `process_next_line` uses `math::min(m_file_size,
  m_current_pos+buffer_size-1)` (the math::min wall is original code, not our idiom);
  flush's copy loop reads/writes `char buffer[4096]` chunks; `m_line_groups` group size 256.

### Callbacks - both sweep verdicts validated as intentional v0.100b changes

* `logging::log_callback` (2011) = `boost::function<void(file, funcsig, line, verbosity,
  string, callback_flag)>`; the static `s_log_callback` in legacy helper.cpp was *used* (fired
  per line from `predicate::operator()`, with `first`/`last` flags computed exactly as today).
  v0.100b widened it to `log_callback_boost` (+user_data, +initiator, +length) and moved
  dispatch into `append`/logger - the leftover `static s_log_callback` in our logger.cpp is
  the orphan of this refactor, which is why the devs "forgot to remove" it.
* `debug::log_callback` (2011) was ALSO `boost::function`, with the **same 4-arg signature**
  `(pcstr initiator, bool is_error_verbosity, bool log_only_user_string, pcstr message)`.
  v0.100b demoted it to a raw fn pointer (proved from bytes in the sweep). History confirms
  the parameter *semantics* we inferred (incl. the `log_only_user_string` arm in
  debug_log_callback) while the raw-pointer typedef stays a real target-era change.
* `verbosity` enum: our current `api.h` enum is character-identical to the OLDEST legacy
  generation (incl. the commented `//1 << n` values and `unset = 1 << 31`). Confirmed.
* `log_flags_enum` as bitmask: legacy `log_thread_unsafe` consumes
  `(log_flags & log_to_console)` / `(log_flags & log_to_stderr)` and has the
  `logged_to_stdout` re-check pattern - ancestors of core `logging_callback`'s bit tests and
  redundant stdout re-check. Confirmed original style.
* `use_console_for_logging`: legacy static-init is `run_tests_command_line() || s_use_console` -
  the REVERSE operand order of the target (`s_use_console || run_tests...`, key first, from
  bytes). The sweep's byte-derived order stands; the swap is a real v0.100b edit.

### Core helper inline-vs-call walls - history refines the hypothesis

Legacy declaration sites (all checked at `72175f57^`):

| helper | legacy status | implication |
|---|---|---|
| `strings::compare_insensitive` | `inline`, `strings_functions(_inline).h` | wall = LTCG link-set, not a moved definition |
| `xray::vsnprintf` | `inline`, `stdlib_extensions.h` (forwards to `_vsnprintf_s`) | same |
| `strings::copy<count>` / `copy_n` | `inline` template, `strings_functions.h` | same |
| `buffer_string::find` / `c_str` | `inline`, `buffer_string.h` | same |
| `threading::interlocked_exchange_pointer(atomic_ptr_type&,pvoid)` | **`XRAY_CORE_API` out-of-line in core**; only the typed template wrapper inline | matches our current header exactly; base-side inlining of it is LTCG cross-TU |
| `math::min` | header template | same |

The README's "the original likely defined several of these out-of-line in core" is NOT
supported by history for the string helpers - they were header-inline in original-dev code
too. The only genuinely out-of-line one (`interlocked_exchange_pointer`) is already declared
out-of-line in our tree. So every wall is a **link-set-dependent LTCG decision**; there is no
core-side declaration fix to adopt, and per the no-cross-unit rule nothing should be edited
from logging. Bank and stop re-deriving.

## Adoption candidates

1. **Close the path_parts open question in README.md** with the m_index/m_current_element
   explanation above (doc-only).
2. **Reclassify `log_file::on_terminate`** from "invented STUB body" to history-confirmed
   original (`{ close(); }` is verbatim legacy); keep declaration + body (doc/comment-only).
3. **Soften `macros.h:58` sushi@NOTE**: history shows the guarded-ternary macro pattern and
   the five dispatch shapes are original; only the per-name macro spelling is unverifiable.
4. **`filter_tree::pop_filter` recovery source**: if a future link ever needs the body, legacy
   `pop_filter` (debug_log_disable_raii, write-lock, pop_back, free with the rule's own
   allocator, `build_tree(globals->tree_allocator)`) is the original to adapt.
5. **`logging_filters_console_command.cpp` reference**: legacy `cc_logging_rule::execute` /
   `save_to` / `rule_name_eq` are the originals for any remaining shape doubts there.
6. **Do NOT chase the inline-vs-call walls from source** - history confirms they are LTCG
   artifacts (see table); no header/core change is justified by the legacy evidence.

Nothing in the legacy tree contradicts a single byte-derived sweep verdict; everywhere the
two disagree (compare_insensitive in string_to_verbosity, flush early-return, raw
debug::log_callback, operand order in use_console_for_logging, char[512] format strings),
the legacy code is simply older and the sweep's reading of the target stands.
