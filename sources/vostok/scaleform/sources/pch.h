// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_SCALEFORM_BUILDING

#include <vostok/macro_platform.h>

#define VOSTOK_LOG_MODULE_INITIATOR	"scaleform"
#include <vostok/extensions.h>

// GFx headers pull in the real <windows.h>; extensions.h (via os_extensions.h)
// leaves HANDLE/HWND/APIENTRY/INVALID_HANDLE_VALUE defined as fake macros that
// corrupt windef.h/rpcasync.h. The os_preinclude->os_include ritual undefs them
// and includes <windows.h> cleanly first (same fix render_include.h applies for
// the render engine, the other in-tree GFx consumer).
#include <vostok/os_preinclude.h>
#undef NOUSER
#undef NOMSG
#undef NOGDI
#undef NOTEXTMETRIC
#include <vostok/os_include.h>

#include "scaleform_memory.h"

using namespace vostok;

#endif // #ifndef PCH_H_INCLUDED
