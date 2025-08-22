////////////////////////////////////////////////////////////////////////////
//	Created		: 15.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_FS_API_H_INCLUDED
#define VOSTOK_FS_API_H_INCLUDED

#ifndef VOSTOK_FS_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_FS_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_FS_BUILDING
#			define VOSTOK_FS_API		VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_FS_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_FS_API	VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_FS_API	VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_FS_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_FS_API

namespace vostok {
namespace fs_new {

typedef memory::doug_lea_allocator_type		allocator_type;

struct world;
struct engine;

VOSTOK_FS_API	world*	create_world	( engine& engine );
VOSTOK_FS_API	void	destroy_world	( world*& world );
VOSTOK_FS_API	void	memory_allocator( allocator_type& allocator );

} // namespace fs_new
} // namespace vostok

#endif // #ifndef VOSTOK_FS_API_H_INCLUDED