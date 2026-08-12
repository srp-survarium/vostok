////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for the survarium animation-controller
// carcass (simple_animation_controller / single_position_animation_controller and
// the animations_search_service A* machinery). These live in the game module and
// have no reachable caller until the human_npc animation-controller graph is wired,
// so /OPT:REF strips them. use_game_animation() references every out-of-line symbol
// so the linker keeps the carcass objects in the base EXE for the delinker to score.
//
// Self-guards like the other anchors: the heavy ctors never actually run (the refs
// are placeholders); the compiler still emits every reference.
//
// Retire once the real human_npc animation-controller call graph reaches these.

#include "pch.h"

#include <vostok/animation/mixing_expression.h>

#include <vostok/configs_binary_config_value.h>

#include "simple_animation_controller.h"
#include "single_position_animation_controller.h"
#include "animations_search_service.h"
#include "animation_space_graph.h"
#include "animation_space_graph_cook.h"
#include "human_npc.h"
#include "animation_controller_parameters.h"

namespace vostok {

#pragma comment( linker, "/include:?get_state_buffer_size@animation_player@animation@vostok@@QBEIXZ" )

void use_game_animation( )
{
	static volatile bool s_run = false;
	if( !s_run )
		return;

	// placeholders sourced through volatile pointers so LTCG cannot const-fold the
	// null args into the carcass bodies (a constant arg folds the match away).
	static survarium::human_npc* volatile					s_owner		= 0;
	static survarium::animation_space_graph_ptr* volatile	s_graph		= 0;
	static vostok::ai::navigation::world* volatile			s_world		= 0;
	static survarium::base_animation_controller* volatile	s_next		= 0;
	static vostok::mutable_buffer* volatile					s_buffer	= 0;
	static survarium::animation_controller_parameters* volatile	s_params = 0;
	static vostok::render::game::renderer* volatile			s_renderer	= 0;
	static vostok::render::scene_ptr* volatile				s_scene		= 0;

	survarium::human_npc&							owner		= *s_owner;
	survarium::animation_space_graph_ptr const&		graph		= *s_graph;
	vostok::ai::navigation::world const&			world		= *s_world;
	survarium::base_animation_controller&			next		= *s_next;
	vostok::mutable_buffer&							buffer		= *s_buffer;
	survarium::animation_controller_parameters const&	params	= *s_params;
	vostok::render::game::renderer&					renderer	= *s_renderer;
	vostok::render::scene_ptr const&				scene		= *s_scene;

	{
		survarium::simple_animation_controller controller( owner );
		controller.initialize( );
		controller.try_finalize( next, buffer );
		controller.selected_animations( buffer );
		controller.set_target( params );
		controller.query_new_target_if_needed( );
		controller.debug_draw( renderer, scene );
	}

	{
		survarium::single_position_animation_controller controller( graph, world, owner );
		controller.initialize( );
		controller.try_finalize( next, buffer );
		controller.selected_animations( buffer );
		controller.set_target( params );
		controller.query_new_target_if_needed( );
		controller.debug_draw( renderer, scene );
	}

	{
		survarium::animations_search_service service;
		service.search( graph, ( survarium::vector< u32 >* )NULL, *( survarium::animation_space_vertex_id* )NULL, *( survarium::animation_space_vertex_id* )NULL );
	}

	// ----- animation_space_graph : the cook allocates the vertex/mix/edge arrays
	// behind the object, so the graph members have no reachable caller until the cook
	// is wired; reference each out-of-line symbol to keep the carcass in the base EXE.
	{
		static vostok::ai::navigation::world* volatile			s_navworld	= 0;
		static vostok::animation::animation_player* volatile	s_player	= 0;
		static float volatile									s_radius	= 0.0f;
		static u32 volatile										s_count		= 0;
		static pcstr volatile									s_path		= 0;
		static survarium::animation_space_vertex const* volatile	s_vertex	= 0;
		vostok::ai::navigation::world&							navworld	= *s_navworld;
		vostok::animation::animation_player&					player		= *s_player;

		survarium::animation_space_graph	space_graph( navworld, s_radius, s_count, s_count, s_count );
		space_graph.edge( s_count );
		space_graph.get_animation_by_path( s_path );
		space_graph.max_speed( );
		survarium::animation_space_graph::get_movement( player, s_vertex, s_vertex, s_radius );

		static vostok::resources::managed_resource_ptr* volatile	s_anim	= 0;
		survarium::animation_space_vertex	vertex( *s_anim, s_path );
		( void )vertex;

		static vostok::configs::binary_config_value const* volatile	s_groups	= 0;
		survarium::get_animation_vertices_count( *s_groups );
		survarium::get_animation_mixes_count( *s_groups );
	}

	// ----- the free comparison operators on the parameter structs have no reachable
	// caller (set_target compares the emitter members inline), so /OPT:REF strips them;
	// reference each through volatile placeholders to keep them in the base EXE.
	{
		static survarium::simple_animation_controller_parameters* volatile	s_simple	= 0;
		static survarium::movement_animation_controller_parameters* volatile	s_movement	= 0;
		static bool volatile												s_sink		= false;

		survarium::simple_animation_controller_parameters const&	simple		= *s_simple;
		survarium::movement_animation_controller_parameters const&	movement	= *s_movement;

		s_sink	= ( simple == simple );
		s_sink	= ( simple != simple );
		s_sink	= ( movement == movement );
		s_sink	= ( movement != movement );
	}
}

} // namespace vostok
