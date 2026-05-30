## Logging

### Matching dept

* Previously there were multiple `operator()` overloads, which allowed `LOG_*` macro to pass flags and log location. With current `append` functions this was not possible and is instead replaced with a menagerie of `LOG_*` macros: `LOG_ERROR`, `LOGF_ERROR`, `LOGFD_ERROR`, `LOGIFD_ERROR` `LOGFD_FORCED`, etc. I don't think this is how they actually wrote this code, but based on the pdb file I cannot assume anything else for now.
* Also those macro are not compiled into the same assembly. This needs to be investigated further.
* Devs forgot to remove `static vostok::logging::log_callback_boost	s_log_callback` in `logger.cpp`. It doesn't seem to be used by anything according to IDA, but it did generate a dynamic constructor function, which is not generated in my case. Why? Maybe I should initialize it to something?

### Open questions

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
