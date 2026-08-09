////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animation_world.h"
#include <vostok/animation/skeleton_animation.h>
#include <vostok/animation/skeleton.h>
#include <vostok/animation/bone_names.h>
#include <vostok/render/world.h>
#include <vostok/render/facade/debug_renderer.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_fs.h>
#include <vostok/animation/engine.h>
#include <vostok/fs_utils.h>

#if	VOSTOK_USE_MAYA_ANIMATION
#	include <vostok/maya_animation/engine.h>
#	include <vostok/maya_animation/api.h>
#endif // #if VOSTOK_USE_MAYA_ANIMATION

#include <vostok/maya_animation/api.h>
namespace vostok {

namespace maya_animation {
	class discrete_data;
} // namespace maya_animation

namespace animation {

command_line::key	test_animation("test_animation", "", "animation", "");

extern command_line::key	check_animation_b_spline_approximation;

animation_world::animation_world(
		vostok::animation::engine& engine,
//		render::scene_ptr const& test_scene,
		render::scene_renderer* scene_renderer,
		render::debug::renderer* debug_renderer
	) :
	m_engine					( engine ),
	m_scene_renderer			( scene_renderer ),
	m_debug_renderer			( debug_renderer )
//	m_test_scene				( test_scene )
{
#if VOSTOK_USE_MAYA_ANIMATION
	if ( check_animation_b_spline_approximation )
		vostok::maya_animation::set_memory_allocator( *vostok::animation::g_allocator );
#endif // #if VOSTOK_USE_MAYA_ANIMATION
}

//void animation_world::set_test_scene( render::scene_ptr const& scene )
//{
//	m_test_scene = scene;
//}

animation_world::~animation_world( )
{
}

void animation_world::tick( )
{
}

bi_spline_skeleton_animation *animation_world::create_skeleton_animation_data( u32 bones_count )
{
		/* sushi@TODO
	bi_spline_skeleton_animation * ret = NEW(bi_spline_skeleton_animation)( );
	ret->set_num_bones( bones_count );
	return ret;
	*/
return NULL;
}

void	animation_world::destroy( bi_spline_skeleton_animation	* &p )
{
		/* sushi@TODO	DELETE( p ); */
}

void animation_world::clear_resources	( )
{
}

} // namespace animation
} // namespace vostok
