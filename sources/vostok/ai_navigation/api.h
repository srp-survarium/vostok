////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_NAVIGATION_API_H_INCLUDED
#define VOSTOK_AI_NAVIGATION_API_H_INCLUDED

#include <vostok/render/engine/base_classes.h>

#ifndef VOSTOK_AI_NAVIGATION_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_AI_NAVIGATION_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_AI_NAVIGATION_BUILDING
#			define VOSTOK_AI_NAVIGATION_API		VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_AI_NAVIGATION_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_AI_NAVIGATION_API	VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_AI_NAVIGATION_API	VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_AI_NAVIGATION_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_AI_NAVIGATION_API

namespace vostok {

namespace render {
namespace debug {
	class renderer;
} // namespace game
} // namespace render

namespace ai {
namespace navigation {

typedef memory::base_allocator	allocator_type;

struct world;
struct engine;

VOSTOK_AI_NAVIGATION_API	world*	create_world		( engine& engine, render::scene_ptr const& scene, vostok::render::debug::renderer& renderer );
VOSTOK_AI_NAVIGATION_API	void	destroy_world		( world*& world );
VOSTOK_AI_NAVIGATION_API	void	set_memory_allocator( allocator_type& allocator );

VOSTOK_AI_NAVIGATION_API bool is_triangle_inside_triangle ( 
	u32 const (&coordinate_indices)[2],
	math::float3 const& v0,
	math::float3 const& v1,
	math::float3 const& v2,
	math::float3 const& u0,
	math::float3 const& u1,
	math::float3 const& u2
);

} // namespace navigation
} // namespace ai
} // namespace vostok

#endif // #ifndef VOSTOK_AI_NAVIGATION_API_H_INCLUDED
