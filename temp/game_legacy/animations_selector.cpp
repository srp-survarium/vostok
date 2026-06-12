////////////////////////////////////////////////////////////////////////////
//	Created		: 21.11.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "animations_selector.h"
#include <vostok/ai/animation_item.h>
#include <vostok/ai/movement_target.h>

namespace survarium {

animations_selector::animations_selector	( vostok::animation::animation_player& player ) :
	m_animation_player						( player ),
	m_current_controller					( 0 ),
	m_target_controller						( 0 ),
	m_target_controller_parameters			( 0 )
{
}

void animations_selector::set_target		( vostok::ai::animation_item const& animation_emitter )
{
	m_simple_animation_parameters.emitter	= static_cast_resource_ptr< animation::animation_expression_emitter_ptr >( animation_emitter.animation );
	
	m_target_controller						= &m_simple_animation_controller;
	m_target_controller_parameters			= &m_simple_animation_parameters;
}

void animations_selector::set_target		( vostok::ai::movement_target const& target_position )
{
	m_movement_animation_parameters.position		= target_position.target_position;
	m_movement_animation_parameters.eyes_direction	= target_position.direction;
	m_movement_animation_parameters.velocity		= target_position.velocity;
	m_movement_animation_parameters.animation		= static_cast_resource_ptr< animation::animation_expression_emitter_ptr >( target_position.preferable_animation->animation );
	
	m_target_controller								= &m_single_position_animation_controller;
	m_target_controller_parameters					= &m_movement_animation_parameters;
}

} // namespace survarium