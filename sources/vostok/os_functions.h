////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.09.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_OS_FUNCTIONS_H_INCLUDED
#define VOSTOK_OS_FUNCTIONS_H_INCLUDED

#include <vostok/macro_building.h>
#include <vostok/type_extensions.h>

namespace vostok {

namespace memory {
	class base_allocator;
} // namespace memory

namespace os {

VOSTOK_CORE_API void copy_to_clipboard	( pcstr buffer );
VOSTOK_CORE_API pstr paste_from_clipboard	( vostok::memory::base_allocator& allocator );
VOSTOK_CORE_API void update_clipboard		( pcstr const string );
VOSTOK_CORE_API bool is_clipboard_empty	( );

} // namespace os
} // namespace vostok

#endif // #ifndef VOSTOK_OS_FUNCTIONS_H_INCLUDED