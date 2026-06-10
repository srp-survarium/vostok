# logging - matching notes

- Namespace: `vostok::logging`.
- Build: `/Od` (optimization off) + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/logging/sources/` (low-level); high-level API in `vostok/core/logging_extensions`.
- Reachability anchor: `use_log` in `game_core/sources/temp_include_all.cpp`.
- Status: nearly complete (1 `STUB` left).

### Module constraints (what a matcher must know going in)

* **The `LOG_*` API is a menagerie of macros, not overloads.** The PDB shows no
  multiple `operator()` overloads (which would have let one macro pass flags and
  log location); each variant is its own macro instead: `LOG_ERROR`, `LOGF_ERROR`,
  `LOGFD_ERROR`, `LOGIFD_ERROR`, `LOGFD_FORCED`, etc. Match against the macro
  expansion, not the naive single-entry-point C++ shape. (Probably not how the
  original authors wrote it, but the PDB supports nothing else for now.)
* **Those macros do NOT all compile to the same assembly** - do not assume a
  shared shape across variants; verify per macro.

### Open questions

* `logger.cpp` keeps a leftover `static vostok::logging::log_callback_boost s_log_callback`
  (the devs forgot to remove it; unused per IDA), whose dynamic-initializer the
  target emits but our build does not - why? Maybe it must be initialized to
  something.
* There are `debug_log` functions, which it seems like I didn't use at all.

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
