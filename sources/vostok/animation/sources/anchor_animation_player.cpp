////////////////////////////////////////////////////////////////////////////
//	Reachability anchor for animation_player's optimized call graph.
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include <vostok/animation/animation_player.h>
#include <vostok/animation/mixing_expression.h>
#include "mixing_n_ary_tree_converter.h"

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

	mixing::n_ary_tree_converter converter( expression );
	static u32 volatile s_buffer_size = 0;
	s_buffer_size = converter.needed_buffer_size( );

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
	animation_player::invert_times( player.m_mixing_tree, s_time, s_sink, s_sink );

	player.set_object_transform( transform, s_object );
	math::float4x4	out	= player.get_object_transform( s_object );
	( void )out;

	player.reset( s_sink );
}

} // namespace vostok
