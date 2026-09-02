// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "simple_animation_controller.h"
// mixing::expression returned by value -> needs the complete type at the definition
#include <vostok/animation/mixing_expression.h>
// m_owner.on_animation_end() needs the complete human_npc
#include "human_npc.h"

namespace survarium {

simple_animation_controller::simple_animation_controller( human_npc& owner ) :
	m_owner( owner ),
	m_last_animation_emitted( false )
{
}

simple_animation_controller::~simple_animation_controller( )
{
}

void simple_animation_controller::initialize( )
{
}

void simple_animation_controller::set_target( animation_controller_parameters const& target )
{
	m_target_parameters										= static_cast_checked< simple_animation_controller_parameters const& >( target );
}

void simple_animation_controller::query_new_target_if_needed( )
{
	if ( m_last_animation_emitted ) {
		// claude@NOTE: target emits a real `call human_npc::on_animation_end`; our base
		// inlines it to nothing because human_npc::on_animation_end is still an empty STUB
		// (human_npc.cpp - a different unit). The call statement is correct; the missing
		// instruction is the empty-stub callee, recovered when human_npc.cpp is matched.
		m_owner.on_animation_end( );
		m_current_parameters.reset( );
		m_target_parameters.reset( );
	}
}

animation::mixing::expression simple_animation_controller::selected_animations( mutable_buffer& buffer )
{
	if ( m_current_parameters.emitter != m_target_parameters.emitter ) {
		m_current_parameters								= m_target_parameters;
		m_last_animation_emitted							= false;
	}

	if ( m_last_animation_emitted )
		return												animation::mixing::expression( );

	return													m_current_parameters.emitter->emit( buffer, m_last_animation_emitted );
}

animation::mixing::expression simple_animation_controller::try_finalize( base_animation_controller& next_controller, mutable_buffer& buffer )
{
	return													animation::mixing::expression( );
}

void simple_animation_controller::debug_draw( render::game::renderer& render, render::scene_ptr const& scene ) const
{
}

} // namespace survarium
