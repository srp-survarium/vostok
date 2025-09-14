////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_API_H_INCLUDED
#define VOSTOK_RENDER_BASE_API_H_INCLUDED

#include <vostok/render/api.h>

#ifndef VOSTOK_RENDER_BASE_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_RENDER_BASE_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_RENDER_BASE_BUILDING
#			define VOSTOK_RENDER_BASE_API			VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_RENDER_BASE_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_RENDER_BASE_API		VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_RENDER_BASE_API		VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_RENDER_BASE_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_RENDER_API

namespace vostok {
namespace render {

typedef memory::base_allocator										logic_allocator_type;
typedef memory::base_allocator										editor_allocator_type;
typedef memory::doug_lea_allocator_type								render_allocator_type;

VOSTOK_RENDER_BASE_API	void	memory_allocator	(
									render_allocator_type& render_allocator,
									logic_allocator_type& logic_allocator,
									editor_allocator_type& editor_allocator
								);

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_BASE_API_H_INCLUDED