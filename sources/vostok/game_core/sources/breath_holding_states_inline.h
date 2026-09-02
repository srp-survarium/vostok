// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef BREATH_HOLDING_STATES_INLINE_H_INCLUDED
#define BREATH_HOLDING_STATES_INLINE_H_INCLUDED

namespace survarium {

inline void breath_state::set_breath_holding_params( breath_holding_params const* params )
{
	m_params = params;
}

inline void breath_state::initialize( )
{
}

inline breath_state::breath_state( float& breath_holding_reserve )
	:	m_breath_holding_reserve( breath_holding_reserve )
{
}

inline bool breath_state_normal::is_ready_for_transition( ) const
{
	return true;
}

inline void breath_state_normal::tick( const float dt )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params != 0 ) );
	m_breath_holding_reserve = math::min( m_breath_holding_reserve + dt, m_params->max_breath_holding_time );
}

inline void breath_state_holding::set_breath_holding_params( breath_holding_params const* params )
{
	m_params = params;
	if ( m_params )
		m_multiplier = params->breath_holding_multiplier;
}

inline void breath_state_holding::tick( const float dt )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params != 0 ) );
	m_breath_holding_reserve = math::max( m_breath_holding_reserve - dt, 0.0f );
}

inline void breath_state_shortbreathing::set_breath_holding_params( breath_holding_params const* params )
{
	m_params = params;
	if ( m_params )
	{
		m_multiplier = m_params->shortbreathing_multiplier;
		m_restoring_speed = m_params->max_breath_holding_time / m_params->shortbreathing_repair_time;
	}
}

inline void breath_state_shortbreathing::tick( const float dt )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params != 0 ) );
	m_breath_holding_reserve = math::min( m_breath_holding_reserve + m_restoring_speed * dt, m_params->max_breath_holding_time );
}

inline bool breath_state_shortbreathing::is_ready_for_transition( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params != 0 ) );
	return m_breath_holding_reserve == m_params->max_breath_holding_time;
}

} // namespace survarium

#endif // #ifndef BREATH_HOLDING_STATES_INLINE_H_INCLUDED
