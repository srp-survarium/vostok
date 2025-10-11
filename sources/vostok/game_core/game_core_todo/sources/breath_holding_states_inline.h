////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BREATH_HOLDING_STATES_INLINE_H_INCLUDED
#define BREATH_HOLDING_STATES_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
// void survarium::breath_state::set_breath_holding_params(survarium::breath_holding_params const*)
void breath_state::set_breath_holding_params( breath_holding_params const* params )
{
	// FUNCTION BODY
	// <0xcb367>|0x000|0x000:'19'
	// ******
}

// STATE[STUB]
// void survarium::breath_state::initialize()
void breath_state::initialize( )
{
}

// STATE[STUB]
// survarium::breath_state::breath_state(float&)
breath_state::breath_state( float& breath_holding_reserve )
{
}

// STATE[STUB]
// bool survarium::breath_state_normal::is_ready_for_transition() const
bool breath_state_normal::is_ready_for_transition( ) const
{
	return false;
}

// STATE[STUB]
// void survarium::breath_state_normal::tick(const float)
void breath_state_normal::tick( float dt )
{
	// FUNCTION BODY
	// <0xcb409>|0x000|0x000:'49'
	// <0xcb415>|0x00c|0x00c:'50'
	// ******
}

// STATE[STUB]
// void survarium::breath_state_holding::set_breath_holding_params(survarium::breath_holding_params const*)
void breath_state_holding::set_breath_holding_params( breath_holding_params const* params )
{
	// FUNCTION BODY
	// <0xcb457>|0x000|0x000:'65'
	// <0xcb460>|0x009|0x009:'66'
	// <0xcb469>|0x012|0x009:'67'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::breath_state_holding::tick(const float)
void breath_state_holding::tick( float dt )
{
	// FUNCTION BODY
	// <0xcb489>|0x000|0x000:'73'
	// <0xcb495>|0x00c|0x00c:'74'
	// ******
}

// STATE[STUB]
// void survarium::breath_state_shortbreathing::set_breath_holding_params(survarium::breath_holding_params const*)
void breath_state_shortbreathing::set_breath_holding_params( breath_holding_params const* params )
{
	// FUNCTION BODY
	// <0xcb4d7>|0x000|0x000:'89'
	// <0xcb4e0>|0x009|0x009:'90'
	// <0xcb4e9>|0x012|0x009:'91'
	// <0xcb4f8>|0x021|0x00f:'92'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::breath_state_shortbreathing::tick(const float)
void breath_state_shortbreathing::tick( float dt )
{
	// FUNCTION BODY
	// <0xcb529>|0x000|0x000:'98'
	// <0xcb535>|0x00c|0x00c:'99'
	// ******
}

// STATE[STUB]
// bool survarium::breath_state_shortbreathing::is_ready_for_transition() const
bool breath_state_shortbreathing::is_ready_for_transition( ) const
{
	return false;
	// FUNCTION BODY
	// <0xcb579>|0x000|0x000:'103'
	// <0xcb585>|0x00c|0x00c:'104'
	// ******
}

} // namespace survarium

#endif // #ifndef BREATH_HOLDING_STATES_INLINE_H_INCLUDED
