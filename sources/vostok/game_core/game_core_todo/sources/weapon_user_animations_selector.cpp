////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_user_animations_selector.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_user_animations_selector::weapon_user_animations_selector()
weapon_user_animations_selector::weapon_user_animations_selector( )
{
	// LOCALS
	// player_logic_base_state* 	crouch
	// player_logic_base_state* 	sprint
	// player_logic_base_state* 	stand
	// player_logic_base_state* 	jumping
	// ******

	// FUNCTION BODY
	// <0x595697>|0x000|0x000:'27'
	// <0x5956fe>|0x067|0x067:'28'
	// <0x595765>|0x0ce|0x067:'29'
	// <0x5957d2>|0x13b|0x06d:'30'
	// 1
	// <0x595839>|0x1a2|0x067:'32'
	// <0x595848>|0x1b1|0x00f:'33'
	// <0x595857>|0x1c0|0x00f:'34'
	// <0x595866>|0x1cf|0x00f:'35'
	// 1
	// <0x595875>|0x1de|0x00f:'37'
	// <0x5958db>|0x244|0x066:'38'
	// <0x595941>|0x2aa|0x066:'39'
	// 1
	// <0x5959b6>|0x31f|0x075:'41'
	// <0x595a2b>|0x394|0x075:'42'
	// 1
	// <0x595aa0>|0x409|0x075:'44'
	// <0x595b15>|0x47e|0x075:'45'
	// <0x595b8a>|0x4f3|0x075:'46'
	// 1
	// <0x595bff>|0x568|0x075:'48'
	// <0x595c74>|0x5dd|0x075:'49'
	// 1
	// <0x595ce9>|0x652|0x075:'51'
	// 1
	// <0x595d5e>|0x6c7|0x075:'53'
	// 1
	// <0x595de5>|0x74e|0x087:'55'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::~weapon_user_animations_selector()
void weapon_user_animations_selector::~weapon_user_animations_selector( )
{
	// LOCALS
	// vostok::ai::fsm_state* 		state<1>
	// ******

	// FUNCTION BODY
	// <0x5950ba>|0x000|0x000|[1]:'60'
	// <0x5950cb>|0x011|0x011:'61'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::tick()
void weapon_user_animations_selector::tick( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58449a }, type_index: TypeIndex(0x916f) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5844d2 }, type_index: TypeIndex(0x916f) })
	// ******

	// FUNCTION BODY
	// <0x595489>|0x000|0x000:'66'
	// 1
	// <0x5954b8>|0x02f|0x02f:'68'
	// 1
	// <0x5954bf>|0x036|0x007:'70'
	// 1
	// <0x59550b>|0x082|0x04c:'72'
	// 1
	// <0x595512>|0x089|0x007:'74'
	// 1
	// ******
}

// STATE[STUB]
// survarium::player_logic_base_state& survarium::weapon_user_animations_selector::current_state() const
player_logic_base_state& weapon_user_animations_selector::current_state( ) const
{
	// LOCALS
	// player_logic_base_state* 	result
	// ******

	// FUNCTION BODY
	// <0x594a39>|0x000|0x000:'80'
	// <0x594a53>|0x01a|0x01a:'81'
	// <0x594a5f>|0x026|0x00c:'82'
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::weapon_user_animations_selector::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> weapon_user_animations_selector::selected_animations( vostok::mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583e80 }, type_index: TypeIndex(0x6ddb) })
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x594e59>|0x000|0x000:'96'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::activate(survarium::base_player&, boost::function<void __cdecl(void)> const&, boost::function<void __cdecl(void)> const&)
void weapon_user_animations_selector::activate( base_player& user, boost::function<void __cdecl(void)> const& sprint_start_callback, boost::function<void __cdecl(void)> const& sprint_end_callback )
{
	// LOCALS
	// vostok::ai::fsm_state* 		i<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583fd9 }, type_index: TypeIndex(0x6dd9) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58405c }, type_index: TypeIndex(0x9182) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x584088 }, type_index: TypeIndex(0x916d) })
	// ******

	// FUNCTION BODY
	// <0x594f9a>|0x000|0x000:'101'
	// <0x594fa3>|0x009|0x009|[1]:'102'
	// <0x594fc4>|0x02a|0x021:'103'
	// 1
	// <0x594fdd>|0x043|0x019:'105'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x594fec>|0x052|0x00f:'113'
	// 1
	// <0x59506e>|0x0d4|0x082:'115'
	// <0x595096>|0x0fc|0x028:'116'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::deactivate()
void weapon_user_animations_selector::deactivate( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583eba }, type_index: TypeIndex(0x9185) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583ed6 }, type_index: TypeIndex(0x916d) })
	// ******

	// FUNCTION BODY
	// <0x594e99>|0x000|0x000:'121'
	// <0x594ea3>|0x00a|0x00a:'122'
	// 1
	// <0x594ebc>|0x023|0x019:'124'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::is_ready_to_be_deactivated() const
bool weapon_user_animations_selector::is_ready_to_be_deactivated( ) const
{
	return false;
	// FUNCTION BODY
	// <0x594c09>|0x000|0x000:'129'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::is_sprinting() const
bool weapon_user_animations_selector::is_sprinting( ) const
{
	return false;
	// FUNCTION BODY
	// <0x594bd9>|0x000|0x000:'134'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::serialize(vostok::network_core::udp_match_packet&) const
void weapon_user_animations_selector::serialize( vostok::network_core::udp_match_packet& packet ) const
{
	// LOCALS
	// u8 							state_id
	// bool 						found
	// vostok::ai::fsm_state const* current
	// vostok::ai::fsm_state const* i<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583e3d }, type_index: TypeIndex(0x6ddd) })
	// ******

	// FUNCTION BODY
	// <0x594dc9>|0x000|0x000:'139'
	// <0x594dcd>|0x004|0x004:'140'
	// <0x594dd1>|0x008|0x004:'141'
	// <0x594dda>|0x011|0x009|[1]:'142'
	// <0x594dff>|0x036|0x025:'143'
	// <0x594e07>|0x03e|0x008:'144'
	// <0x594e0b>|0x042|0x004:'145'
	// 1
	// <0x594e0d>|0x044|0x002:'147'
	// 1
	// <0x594e0f>|0x046|0x002:'149'
	// 1
	// <0x594e1b>|0x052|0x00c:'151'
	// <0x594e28>|0x05f|0x00d:'152'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::deserialize(vostok::network_core::packet_reader&)
void weapon_user_animations_selector::deserialize( vostok::network_core::packet_reader& reader )
{
	// LOCALS
	// u8 							target_state_id
	// u8 							state_id
	// vostok::ai::fsm_state* 		current
	// vostok::ai::fsm_state* 		i<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583dac }, type_index: TypeIndex(0x6dde) })
	// ******

	// FUNCTION BODY
	// <0x594d29>|0x000|0x000:'157'
	// <0x594d34>|0x00b|0x00b:'158'
	// <0x594d38>|0x00f|0x004:'159'
	// <0x594d3f>|0x016|0x007|[1]:'160'
	// <0x594d69>|0x040|0x02a:'161'
	// <0x594d75>|0x04c|0x00c:'162'
	// <0x594d7b>|0x052|0x006:'163'
	// 1
	// <0x594d7d>|0x054|0x002:'165'
	// 1
	// <0x594d7f>|0x056|0x002:'167'
	// <0x594d8b>|0x062|0x00c:'168'
	// <0x594d97>|0x06e|0x00c:'169'
	// ******
}

// STATE[STUB]
// survarium::weapon_user_state_enum survarium::weapon_user_animations_selector::get_current_state_id() const
weapon_user_state_enum weapon_user_animations_selector::get_current_state_id( ) const
{
	// FUNCTION BODY
	// <0x594a09>|0x000|0x000:'174'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::sprint_predicate() const
bool weapon_user_animations_selector::sprint_predicate( ) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5843f0 }, type_index: TypeIndex(0x916d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x584433 }, type_index: TypeIndex(0x916f) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x584454 }, type_index: TypeIndex(0x9176) })
	// ******

	return false;
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// <0x5953b9>|0x000|0x000:'190'
	// 1
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::stand_predicate() const
bool weapon_user_animations_selector::stand_predicate( ) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58456b }, type_index: TypeIndex(0x916d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5845a4 }, type_index: TypeIndex(0x916f) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5845d3 }, type_index: TypeIndex(0x9173) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x595529>|0x000|0x000:'196'
	// 1
	// <0x595553>|0x02a|0x02a:'198'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x59555a>|0x031|0x007:'205'
	// 1
	// <0x5955f6>|0x0cd|0x09c:'207'
	// 1
	// 2
	// 3
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::set_animation_callback(char const*, void const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&)
void weapon_user_animations_selector::set_animation_callback( pcstr channel_id, void const* callback_uid, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const& animation_callback )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583f7b }, type_index: TypeIndex(0x9183) })
	// ******

	// FUNCTION BODY
	// <0x594f49>|0x000|0x000:'215'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::remove_animation_callback(char const*, void const*)
void weapon_user_animations_selector::remove_animation_callback( pcstr channel_id, void const* callback_uid )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583950 }, type_index: TypeIndex(0x9186) })
	// ******

	// FUNCTION BODY
	// <0x594937>|0x000|0x000:'220'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::set_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&)
void weapon_user_animations_selector::set_animation_callback( vostok::animation::reserved_channel_ids_enum channel_id, void const* callback_uid, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const& animation_callback )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583f26 }, type_index: TypeIndex(0x9182) })
	// ******

	// FUNCTION BODY
	// <0x594ef9>|0x000|0x000:'225'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::remove_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*)
void weapon_user_animations_selector::remove_animation_callback( vostok::animation::reserved_channel_ids_enum channel_id, void const* callback_uid )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583920 }, type_index: TypeIndex(0x9185) })
	// ******

	// FUNCTION BODY
	// <0x594907>|0x000|0x000:'230'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::crouch_predicate() const
bool weapon_user_animations_selector::crouch_predicate( ) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583b79 }, type_index: TypeIndex(0x916f) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583ba8 }, type_index: TypeIndex(0x9173) })
	// ******

	return false;
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x594b59>|0x000|0x000:'243'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::broken_legs_predicate() const
bool weapon_user_animations_selector::broken_legs_predicate( ) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583b1a }, type_index: TypeIndex(0x916d) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x594b09>|0x000|0x000:'248'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::jump_predicate() const
bool weapon_user_animations_selector::jump_predicate( ) const
{
	// LOCALS
	// bool 						result
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5842c0 }, type_index: TypeIndex(0x916d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5842fc }, type_index: TypeIndex(0x916f) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58432e }, type_index: TypeIndex(0x9173) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58434d }, type_index: TypeIndex(0x9176) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x584370 }, type_index: TypeIndex(0x9176) })
	// ******

	return false;
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x595289>|0x000|0x000:'261'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// <0x5953a7>|0x11e|0x11e:'274'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::is_weapon_in_idle() const
bool weapon_user_animations_selector::is_weapon_in_idle( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5951f9>|0x000|0x000:'279'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::is_weapon_firing() const
bool weapon_user_animations_selector::is_weapon_firing( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5951a9>|0x000|0x000:'284'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::is_weapon_toggling() const
bool weapon_user_animations_selector::is_weapon_toggling( ) const
{
	return false;
	// FUNCTION BODY
	// <0x595129>|0x000|0x000:'289'
	// ******
}

// STATE[STUB]
// float survarium::weapon_user_animations_selector::look_time_factor() const
float weapon_user_animations_selector::look_time_factor( ) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x583ad1 }, type_index: TypeIndex(0x9171) })
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x594aa9>|0x000|0x000:'301'
	// ******
}

// STATE[STUB]
// float survarium::weapon_user_animations_selector::look_time_factor_calculator(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float) const
float weapon_user_animations_selector::look_time_factor_calculator(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale) const
{
	return 0.0f;
	// FUNCTION BODY
	// <0x594c99>|0x000|0x000:'313'
	// <0x594cda>|0x041|0x041:'314'
	// ******
}

// STATE[STUB]
// fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> survarium::weapon_user_animations_selector::look_time_calculator() const
fastdelegate::FastDelegate<float __cdecl(float,float,u32,u32,u32,float)> weapon_user_animations_selector::look_time_calculator( ) const
{
	// FUNCTION BODY
	// <0x594cf7>|0x000|0x000:'319'
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_user_animations_selector::on_interval_ended(vostok::animation::animation_callback_params&)
vostok::animation::callback_return_type_enum weapon_user_animations_selector::on_interval_ended( vostok::animation::animation_callback_params& params )
{
	// CONSTANTS
	// const weapon_user_animations_selector::on_interval_ended::__l9::<unnamed-tag> c_right_leg_interval_id = 0;
	// ******

	// FUNCTION BODY
	// <0x5949c9>|0x000|0x000:'324'
	// <0x5949d5>|0x00c|0x00c:'325'
	// 1
	// <0x5949de>|0x015|0x009|[1]:'327'
	// 1
	// 2
	// 3
	// 4
	// <0x5949f1>|0x028|0x013:'332'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::on_broken_limb_affect(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum)
void weapon_user_animations_selector::on_broken_limb_affect( pcstr bodypart, hit_affects_type_enum affect, affect_event_type_enum type )
{
	// FUNCTION BODY
	// <0x594969>|0x000|0x000:'337'
	// 1
	// <0x59498c>|0x023|0x023:'339'
	// 1
	// <0x5949af>|0x046|0x023:'341'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::is_in_jump() const
bool weapon_user_animations_selector::is_in_jump( ) const
{
	return false;
	// FUNCTION BODY
	// <0x594a79>|0x000|0x000:'346'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::set_sprint_callbacks(boost::function<void __cdecl(void)> const&, boost::function<void __cdecl(void)> const&)
void weapon_user_animations_selector::set_sprint_callbacks( boost::function<void __cdecl(void)> const& start_callback, boost::function<void __cdecl(void)> const& end_callback )
{
	// LOCALS
	// player_logic_base_state* 	state
	// ******

	// FUNCTION BODY
	// <0x594c39>|0x000|0x000:'351'
	// <0x594c54>|0x01b|0x01b:'352'
	// <0x594c5a>|0x021|0x006:'353'
	// <0x594c69>|0x030|0x00f:'354'
	// 1
	// <0x594c79>|0x040|0x010:'356'
	// <0x594c88>|0x04f|0x00f:'357'
	// ******
}

} // namespace survarium
