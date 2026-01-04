////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Description : physics API
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_API_H_INCLUDED
#define VOSTOK_PHYSICS_API_H_INCLUDED
#include <vostok/render/engine/base_classes.h>

#ifndef VOSTOK_PHYSICS_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_PHYSICS_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_PHYSICS_BUILDING
#			define VOSTOK_PHYSICS_API				VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_PHYSICS_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_PHYSICS_API			VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_PHYSICS_API			VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_PHYSICS_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_PHYSICS_API

namespace vostok {

namespace render {
	namespace debug {
		class renderer;
	} // namespace debug
	class world;
} // namespace render

namespace physics {

struct engine;
struct world;

VOSTOK_PHYSICS_API	world*	create_world_bt				( vostok::memory::base_allocator* allocator, engine& engine );
VOSTOK_PHYSICS_API	void	destroy_world				( vostok::memory::base_allocator* allocator, world* w );

VOSTOK_PHYSICS_API	void	set_memory_allocator		( vostok::memory::base_allocator* allocator );
VOSTOK_PHYSICS_API	void	setup_game_material_groups	( u16 const* game_material_groups, u16 game_materials_count );


} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_API_H_INCLUDED
