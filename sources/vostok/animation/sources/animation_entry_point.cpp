////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/api.h>
#include <vostok/linkage_helper.h>
#include <vostok/math_float4x4.h>
#include "mixing_binary_tree_writer.h"
#include "skeleton_cook.h"
#include "single_animation_cook.h"
#include "animation_collection_cook.h"

#ifndef MASTER_GOLD
#	include "animation_world.h"
#if !defined(VOSTOK_GAME_BUILD)
#	include "editor_animation.h"
#	include "editor_mixer.h"
#endif // #if !defined(VOSTOK_GAME_BUILD)
#endif // #ifndef MASTER_GOLD

VOSTOK_DECLARE_LINKAGE_ID(animation_entry_point)

#pragma comment( linker, "/include:??_7skeleton_animation_cook@animation@vostok@@6B@" )
#pragma comment( linker, "/include:??_7bi_spline_skeleton_animation_impl_cook@animation@vostok@@6B@" )
#pragma comment( linker, "/include:??_7bi_spline_skeleton_animation_baked_cook@animation@vostok@@6B@" )
#pragma comment( linker, "/include:??_7cubic_spline_skeleton_animation_cook@animation@vostok@@6B@" )

vostok::animation::allocator_type*	vostok::animation::g_allocator = 0;

namespace vostok {
namespace animation {

command_line::key test_animation( "test_animation", "", "animation", "" );

static skeleton_cook s_skeleton_cook;
static single_animation_cook s_single_animation_cook;
static animation_collection_cook s_animation_collection_cook;

static bool is_enum_exported = false;

#ifndef MASTER_GOLD
static configs::enum_tuple animation_tuples[] =
{
	std::make_pair( "single_animation_type",		resources::single_animation_class	  ),
	std::make_pair( "composite_animation_type",		resources::composite_animation_class  ),
	std::make_pair( "collection_animation_type",	resources::animation_collection_class )
};
#endif // #ifndef MASTER_GOLD

} // namespace animation
} // namespace vostok

#ifndef MASTER_GOLD

using vostok::animation::animation_world;

static vostok::uninitialized_reference< animation_world > s_world;

vostok::animation::world* vostok::animation::create_world(
		engine& engine,
//		render::scene_ptr const& test_scene,
		render::scene_renderer* scene_renderer,
		render::debug::renderer* debug_renderer
	)
{
#ifndef MASTER_GOLD	
	if ( !vostok::animation::is_enum_exported )
	{
		vostok::configs::export_enum( "animation_resources", vostok::animation::animation_tuples, vostok::animation::animation_tuples + array_size( vostok::animation::animation_tuples ) );
		vostok::animation::is_enum_exported = true;
	}
#endif // #ifndef MASTER_GOLD

	VOSTOK_CONSTRUCT_REFERENCE	( s_world, animation_world )( engine, /*test_scene, */scene_renderer, debug_renderer );
	return						( s_world.c_ptr() );
}

void vostok::animation::destroy_world( vostok::animation::world*& world )
{
	R_ASSERT					( s_world.c_ptr() == world );
	VOSTOK_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

#endif // #ifndef MASTER_GOLD

void vostok::animation::set_memory_allocator( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
}

vostok::math::float4x4 vostok::animation::calculated_head_matrix( vostok::math::float4x4 const& head_matrix, vostok::math::float4x4 const& object_matrix )
{
	math::float4x4 result = math::mul4x3( math::mul4x3( math::mul4x3( math::create_rotation( math::float3( 0.f, math::pi_d2, 0.f ) ), math::create_rotation( math::float3( 0.f, 0.f, math::pi_d2 ) ) ), head_matrix ), object_matrix );
	result.c.xyz( ) += result.j.xyz( ) * .1f;
	result.c.xyz( ) += result.i.xyz( ) * 0.f;
	result.c.xyz( ) += result.k.xyz( ) * 0.f;
	return result;
}

#if !defined(MASTER_GOLD) && !defined(VOSTOK_GAME_BUILD)
vostok::animation::i_editor_animation* vostok::animation::create_editor_animation( allocator_type& allocator, skeleton_animation_ptr const& anim )
{
	return VOSTOK_NEW_IMPL( allocator, editor_animation )( anim );
}

vostok::animation::i_editor_animation* vostok::animation::create_editor_animation( allocator_type& allocator, pcstr anim_file )
{
	editor_animation* anim = VOSTOK_NEW_IMPL(allocator, editor_animation)( anim_file );
	if (!anim->is_exist())
	{
		//destroy_editor_animation(allocator, anim);
		VOSTOK_DELETE_IMPL( allocator, anim );
		return 0;
	}
	return anim;
}

vostok::animation::i_editor_animation* vostok::animation::create_editor_animation( pcstr anim_file )
{
	return create_editor_animation( *g_allocator, anim_file );
}

void vostok::animation::destroy_editor_animation(  i_editor_animation* &anim )
{
	destroy_editor_animation( *g_allocator, anim );
}

void vostok::animation::destroy_editor_animation( allocator_type& allocator, i_editor_animation* &anim )
{
	vostok::animation::editor_animation* a = static_cast<vostok::animation::editor_animation*>(anim);
	VOSTOK_DELETE_IMPL( allocator, a );
	anim = 0;
}

vostok::animation::i_editor_mixer* vostok::animation::create_editor_mixer( allocator_type& allocator, animation_player& animation_player )
{
	return VOSTOK_NEW_IMPL( allocator, editor_mixer )( allocator, animation_player );
}

void vostok::animation::destroy_editor_mixer( allocator_type& allocator, i_editor_mixer* mxr )
{
	vostok::animation::editor_mixer* m = static_cast<vostok::animation::editor_mixer*>(mxr);
	VOSTOK_DELETE_IMPL( allocator, m );
}

vostok::animation::mixing::binary_tree_visitor* vostok::animation::create_binary_tree_writer( allocator_type& allocator, vostok::configs::lua_config_value& cfg, float2& pos )
{
	return VOSTOK_NEW_IMPL(allocator, vostok::animation::mixing::binary_tree_writer)(cfg, pos);
}

void vostok::animation::destroy_binary_tree_writer( allocator_type& allocator, vostok::animation::mixing::binary_tree_visitor*& wr )
{
	vostok::animation::mixing::binary_tree_writer* bwr = static_cast<vostok::animation::mixing::binary_tree_writer*>(wr);
	VOSTOK_DELETE_IMPL(allocator, bwr);
	wr = 0;
}

#endif // #if !defined(MASTER_GOLD) && !defined(VOSTOK_GAME_BUILD)
