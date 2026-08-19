////////////////////////////////////////////////////////////////////////////
//	Created 	: 27.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PLATFORM_EXTENSIONS_WIN_H_INCLUDED
#define PLATFORM_EXTENSIONS_WIN_H_INCLUDED

namespace vostok {

extern pcstr g_delay_loading_libraries_reference_module;

namespace platform {

bool   unload_delay_loaded_library (pcstr dll_name);
void   log_error (pcstr format, pcstr argument);

} // namespace platform
} // namespace vostok

#endif // #ifndef PLATFORM_EXTENSIONS_WIN_H_INCLUDED
