////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_API_H_INCLUDED
#define VOSTOK_RENDER_API_H_INCLUDED

#ifndef VOSTOK_RENDER_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_RENDER_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_RENDER_BUILDING
#			define VOSTOK_RENDER_API				VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_RENDER_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_RENDER_API			VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_RENDER_API			VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_RENDER_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_RENDER_API

#if VOSTOK_PLATFORM_PS3
	typedef pvoid	HWND;
#endif // #if VOSTOK_PLATFORM_PS3

namespace vostok {
namespace particle {
	struct world;
} // namespace particle
} // namespace vostok

namespace vostok {
namespace render { 

class world;

typedef memory::base_allocator					logic_allocator_type;
typedef memory::base_allocator					editor_allocator_type;
typedef memory::doug_lea_allocator_type			render_allocator_type;

VOSTOK_RENDER_API	void	set_memory_allocator	(
							render_allocator_type& render_allocator,
							logic_allocator_type& logic_allocator,
							editor_allocator_type& editor_allocator
						);
VOSTOK_RENDER_API	world*	create_world			(
							memory::base_allocator& logic_allocator,
							memory::base_allocator* editor_allocator
						);
VOSTOK_RENDER_API	void	destroy_world			(
							world*& world
						);

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_API_H_INCLUDED