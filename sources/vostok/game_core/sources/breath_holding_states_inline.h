////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BREATH_HOLDING_STATES_INLINE_H_INCLUDED
#define BREATH_HOLDING_STATES_INLINE_H_INCLUDED

namespace survarium {

// STATE[100%|DONE]
// void survarium::breath_state::set_breath_holding_params(survarium::breath_holding_params const*)
inline void breath_state::set_breath_holding_params( breath_holding_params const* params )
{
	m_params = params;
}

// STATE[100%|DONE]
// void survarium::breath_state::initialize()
inline void breath_state::initialize( )
{
}

// STATE[100%|DONE]
// survarium::breath_state::breath_state(float&)
inline breath_state::breath_state( float& breath_holding_reserve )
	:	m_breath_holding_reserve( breath_holding_reserve )
{
}

// STATE[100%|DONE]
// bool survarium::breath_state_normal::is_ready_for_transition() const
inline bool breath_state_normal::is_ready_for_transition( ) const
{
	return true;
}

// STATE[100%|DONE]
// void survarium::breath_state_normal::tick(const float)
inline void breath_state_normal::tick( const float dt )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params != 0 ) );
	m_breath_holding_reserve = math::min( m_params->max_breath_holding_time, m_breath_holding_reserve + dt );
}

// STATE[100%|DONE]
// void survarium::breath_state_holding::set_breath_holding_params(survarium::breath_holding_params const*)
inline void breath_state_holding::set_breath_holding_params( breath_holding_params const* params )
{
	m_params = params;
	if ( m_params )
		m_multiplier = params->breath_holding_multiplier;
}

// STATE[100%|DONE]
// void survarium::breath_state_holding::tick(const float)
inline void breath_state_holding::tick( const float dt )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params != 0 ) );
	m_breath_holding_reserve = math::max( m_breath_holding_reserve - dt, 0.0f );
}

// STATE[100%|DONE]
// void survarium::breath_state_shortbreathing::set_breath_holding_params(survarium::breath_holding_params const*)
inline void breath_state_shortbreathing::set_breath_holding_params( breath_holding_params const* params )
{
	m_params = params;
	if ( m_params )
	{
		m_multiplier = m_params->shortbreathing_multiplier;
		m_restoring_speed = m_params->max_breath_holding_time / m_params->shortbreathing_repair_time;
	}
}

// STATE[100%|DONE]
// void survarium::breath_state_shortbreathing::tick(const float)
inline void breath_state_shortbreathing::tick( const float dt )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params != 0 ) );
	m_breath_holding_reserve = math::min( m_params->max_breath_holding_time, m_restoring_speed * dt + m_breath_holding_reserve );
}

// STATE[100%|DONE]
// bool survarium::breath_state_shortbreathing::is_ready_for_transition() const
inline bool breath_state_shortbreathing::is_ready_for_transition( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params != 0 ) );
	return m_breath_holding_reserve == m_params->max_breath_holding_time;
}

} // namespace survarium

#endif // #ifndef BREATH_HOLDING_STATES_INLINE_H_INCLUDED
