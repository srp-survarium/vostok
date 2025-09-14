////////////////////////////////////////////////////////////////////////////
//	Created 	: 22.12.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_API_H_INCLUDED
#define VOSTOK_SOUND_API_H_INCLUDED

#ifndef VOSTOK_SOUND_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_SOUND_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_SOUND_BUILDING
#			define VOSTOK_SOUND_API					VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_SOUND_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_SOUND_API				VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_SOUND_API				VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_SOUND_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_SOUND_API

namespace vostok {
namespace sound {

struct engine;
struct world;

typedef ::vostok::memory::base_allocator			base_allocator_type;
typedef memory::doug_lea_allocator_type			allocator_type;

VOSTOK_SOUND_API	world*	create_world			( engine& engine, base_allocator_type& logic_allocator, base_allocator_type* editor_allocator );
VOSTOK_SOUND_API	void	destroy_world			( world*& world );
VOSTOK_SOUND_API	void	set_memory_allocator	( allocator_type& allocator );

} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_API_H_INCLUDED