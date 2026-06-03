////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BREATH_HOLDING_STATES_INLINE_H_INCLUDED
#define BREATH_HOLDING_STATES_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
// void survarium::breath_state::set_breath_holding_params(survarium::breath_holding_params const*)
inline void breath_state::set_breath_holding_params( breath_holding_params const* params )
{
	// FUNCTION BODY[0xcb360]: 1
	// <0xcb367>|0x007|+0x009:'19'
	// ******
}

// STATE[STUB]
// void survarium::breath_state::initialize()
inline void breath_state::initialize( )
{
	// FUNCTION BODY[0x2a800]
	// <0x2a800>|0x000|      :'23'	{
	// ******
}

// STATE[STUB]
// survarium::breath_state::breath_state(float&)
inline breath_state::breath_state( float& breath_holding_reserve )
{
	// FUNCTION BODY[0xcb380]
	// <0xcb380>|0x000|      :'29'	{
	// ******
}

// STATE[STUB]
// bool survarium::breath_state_normal::is_ready_for_transition() const
inline bool breath_state_normal::is_ready_for_transition( ) const
{
	return false;

	// FUNCTION BODY[0xe23e0]
	// <0xe23e0>|0x000|      :'46'	{
	// ******
}

// STATE[STUB]
// void survarium::breath_state_normal::tick(const float)
inline void breath_state_normal::tick( float dt )
{
	// FUNCTION BODY[0xcb400]: 2
	// <0xcb409>|0x009|+0x00c:'49'
	// <0xcb415>|0x015|+0x028:'50'
	// ******
}

// STATE[STUB]
// void survarium::breath_state_holding::set_breath_holding_params(survarium::breath_holding_params const*)
inline void breath_state_holding::set_breath_holding_params( breath_holding_params const* params )
{
	// FUNCTION BODY[0xcb450]: 4
	// <0xcb457>|0x007|+0x009:'65'
	// <0xcb460>|0x010|+0x009:'66'
	// <0xcb469>|0x019|+0x00c:'67'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::breath_state_holding::tick(const float)
inline void breath_state_holding::tick( float dt )
{
	// FUNCTION BODY[0xcb480]: 2
	// <0xcb489>|0x009|+0x00c:'73'
	// <0xcb495>|0x015|+0x026:'74'
	// ******
}

// STATE[STUB]
// void survarium::breath_state_shortbreathing::set_breath_holding_params(survarium::breath_holding_params const*)
inline void breath_state_shortbreathing::set_breath_holding_params( breath_holding_params const* params )
{
	// FUNCTION BODY[0xcb4d0]: 5
	// <0xcb4d7>|0x007|+0x009:'89'
	// <0xcb4e0>|0x010|+0x009:'90'
	// <0xcb4e9>|0x019|+0x00f:'91'
	// <0xcb4f8>|0x028|+0x01d:'92'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::breath_state_shortbreathing::tick(const float)
inline void breath_state_shortbreathing::tick( float dt )
{
	// FUNCTION BODY[0xcb520]: 2
	// <0xcb529>|0x009|+0x00c:'98'
	// <0xcb535>|0x015|+0x030:'99'
	// ******
}

// STATE[STUB]
// bool survarium::breath_state_shortbreathing::is_ready_for_transition() const
inline bool breath_state_shortbreathing::is_ready_for_transition( ) const
{
	return false;

	// FUNCTION BODY[0xcb570]: 2
	// <0xcb579>|0x009|+0x00c:'103'
	// <0xcb585>|0x015|+0x02c:'104'
	// ******
}

} // namespace survarium

#endif // #ifndef BREATH_HOLDING_STATES_INLINE_H_INCLUDED
