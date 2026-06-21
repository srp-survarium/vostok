////////////////////////////////////////////////////////////////////////////
//	/OPT:REF reachability anchor for the animation_player carcass.
////////////////////////////////////////////////////////////////////////////

// animation_player's public surface (set_target_and_tick / subscribe / unsubscribe
// / serialize/deserialize / object-transform accessors) is only reached by the game
// animation-controller graph (not fully wired in this build), so /OPT:REF strips the
// class methods from the base EXE and the delinker never produces bodies to pair
// against the optimized target. anchor_animation_player() constructs an animation_player
// and calls its public surface; the public entry points reach the private callees
// (set_target, compact_callbacks, get_next_buffer, try_get_transform, skip_time_if_needed,
// invert_times) so LTCG emits them out-of-line for the delinker to score.
//
// Self-guarded: the volatile guard is never true at runtime, but the compiler still
// emits every reference, and every argument is sourced through a volatile placeholder
// so LTCG cannot const-fold the carcass bodies away.
//
// Dispatched from survarium::IncludeAll::IncludeAll() (game_core/sources/anchor.cpp).
// TEMPORARY - retire once the real game animation-controller call graph reaches the player.

#include "pch.h"

#include <vostok/animation/animation_player.h>
#include <vostok/animation/mixing_expression.h>

namespace vostok {

void anchor_animation_player( )
{
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	using namespace animation;

	static animation_player* volatile	s_player	= 0;
	animation_player&					player		= *s_player;

	static mixing::expression const* volatile							s_expression	= 0;
	static u32 volatile													s_time			= 0;
	static boost::function< math::float4x4( pcvoid ) > const* volatile	s_functor		= 0;
	static math::float4x4 const* volatile								s_transform		= 0;
	static bool volatile												s_sink			= false;

	mixing::expression const&							expression	= *s_expression;
	boost::function< math::float4x4( pcvoid ) > const&	functor		= *s_functor;
	math::float4x4 const&								transform	= *s_transform;

	s_sink	= player.tick( s_time );
	s_sink	= player.tick_to_nearest_user_handled_callback( s_time );
	s_sink	= player.set_target_and_tick( expression, s_time, functor );
	s_sink	= player.set_target_and_tick( expression, s_time, transform );

	static new_callback_type const* volatile					s_callback	= 0;
	static pcvoid volatile										s_uid		= 0;
	static resources::managed_resource_ptr const* volatile		s_animation	= 0;
	static u8 volatile											s_event		= 0;
	static pcvoid volatile										s_object	= 0;

	new_callback_type const&					callback	= *s_callback;
	resources::managed_resource_ptr const&		animation	= *s_animation;

	player.subscribe( (pcstr)s_uid, callback, s_uid, animation, s_event, s_object );
	player.subscribe( channel_id_on_animation_end, callback, s_uid, animation, s_object );
	player.unsubscribe( (pcstr)s_uid, s_uid );
	player.unsubscribe( channel_id_on_animation_end, s_uid );

	static void* volatile	s_buffer	= 0;
	player.serialize_state( s_buffer, s_time );
	player.deserialize_state( s_buffer, s_time );

	player.set_object_transform( transform, s_object );
	math::float4x4	out	= player.get_object_transform( s_object );
	( void )out;

	player.reset( s_sink );
}

} // namespace vostok
