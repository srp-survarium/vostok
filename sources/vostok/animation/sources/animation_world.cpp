////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animation_world.h"
#include "bi_spline_skeleton_animation.h"
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
#include "skeleton_animation_cook.h"
#include "cubic_spline_skeleton_animation_cook.h"
#include "skeleton_cook.h"
#include "single_animation_cook.h"
#include "animation_collection_cook.h"

namespace vostok {

namespace maya_animation {
	class discrete_data;
} // namespace maya_animation

namespace animation {

command_line::key	test_animation("test_animation", "", "animation", "");

extern command_line::key	check_animation_b_spline_approximation;

animation_world::animation_world(
		vostok::animation::engine& engine,
		render::scene_ptr const& test_scene,
		render::scene_renderer* scene_renderer,
		render::debug::renderer* debug_renderer
	) :
	m_engine					( engine ),
	m_scene_renderer			( scene_renderer ),
	m_debug_renderer			( debug_renderer ),
	m_test_scene				( test_scene )
{
	static skeleton_animation_cook s_skeleton_animation_cook( *this );
	static cubic_spline_skeleton_animation_cook s_cubic_spline_skeleton_animation_cook;
	static skeleton_cook s_skeleton_cook;
	static single_animation_cook s_single_animation_cook;
	static animation_collection_cook s_animation_collection_cook;

#if VOSTOK_USE_MAYA_ANIMATION
	if ( check_animation_b_spline_approximation )
		vostok::maya_animation::set_memory_allocator( *vostok::animation::g_allocator );
#endif // #if VOSTOK_USE_MAYA_ANIMATION
}

void animation_world::set_test_scene( render::scene_ptr const& scene )
{
	m_test_scene = scene;
}

animation_world::~animation_world( )
{
}

void animation_world::tick( )
{
}

bi_spline_skeleton_animation *animation_world::create_skeleton_animation_data( u32 bones_count )
{
	bi_spline_skeleton_animation * ret = NEW(bi_spline_skeleton_animation)( );
	ret->set_num_bones( bones_count );
	return ret;
}

void	animation_world::destroy( bi_spline_skeleton_animation	* &p )
{
	DELETE( p );
}

void animation_world::clear_resources	( )
{
}

} // namespace animation
} // namespace vostok