////////////////////////////////////////////////////////////////////////////
//	Created		: 15.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_VFS_API_H_INCLUDED
#define VOSTOK_VFS_API_H_INCLUDED

#ifndef VOSTOK_VFS_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_VFS_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_VFS_BUILDING
#			define VOSTOK_VFS_API		VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_VFS_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_VFS_API	VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_VFS_API	VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_VFS_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_VFS_API

namespace vostok {
namespace vfs {

// used only in tests
typedef memory::pthreads_allocator_type		allocator_type;

struct world;
struct engine;

VOSTOK_VFS_API	void	temp_helper		( );

VOSTOK_VFS_API	world*	create_world	( engine& engine );
VOSTOK_VFS_API	void	destroy_world	( world*& world );
VOSTOK_VFS_API	void	memory_allocator( allocator_type& allocator );

} // namespace vfs
} // namespace vostok

#endif // #ifndef VOSTOK_VFS_API_H_INCLUDED