////////////////////////////////////////////////////////////////////////////
//	libgfx - forced-include preamble for the Scaleform GFx 4.2.22 SDK TUs.
//
//	Survarium built libgfx.lib from their PATCHED 4.2.22 source as part of their
//	own MASTER_GOLD build. The one libgfx byte-affecting patch
//	(Src/Kernel/HeapMH/HeapMH_SysAllocMalloc.h) routes the Win32 SysAllocMalloc
//	through vostok::memory::g_mt_allocator via VOSTOK_MALLOC_IMPL, so every GFx
//	TU must see the engine memory machinery (memory_macros.h, g_mt_allocator,
//	pvoid, VOSTOK_UNREACHABLE_CODE). <vostok/extensions.h> provides all of it.
//
//	The GFx headers transitively pull the real <windows.h>; extensions.h (via
//	os_extensions.h) leaves HANDLE/HWND/APIENTRY/... as fake macros that corrupt
//	windef.h/rpcasync.h. The os_preinclude -> os_include ritual undefs them and
//	includes <windows.h> cleanly first (same fix the scaleform module and the
//	render engine apply, the other in-tree GFx consumers).
//
//	This file is /FI-forced into the SDK TUs (which are upstream Scaleform source
//	we do not modify), not #include'd by them.
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LIBGFXEXPAT_PCH_H_INCLUDED
#define VOSTOK_LIBGFXEXPAT_PCH_H_INCLUDED

#include <vostok/macro_platform.h>

#define VOSTOK_LOG_MODULE_INITIATOR	"libgfx"
#include <vostok/extensions.h>

#include <vostok/os_preinclude.h>
#undef NOUSER
#undef NOMSG
#undef NOGDI
#undef NOTEXTMETRIC
// The engine os_preinclude lean config drops Win32 APIs the GFx Kernel TUs use
// directly: SF_Locale.cpp needs LCTYPE / GetLocaleInfoW (NONLS guards winnls.h
// out of windows.h), SF_Timer.cpp needs timeGetTime / timeBeginPeriod. winnls.h
// is pulled by windows.h only when NONLS is undefined AT THE windows.h include
// (os_include.h), so undef it here, before that include - undefining it after
// is too late (winnls.h was already skipped).
#undef NONLS
#include <vostok/os_include.h>

// mmsystem.h is not gated by a NO* macro; include it for the multimedia timer.
#include <mmsystem.h>

#endif // #ifndef VOSTOK_LIBGFXEXPAT_PCH_H_INCLUDED
