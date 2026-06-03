////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/game_core/player_logic_base_state.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/player_input.h>

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
	// <0x595697>|0x087|+0x067:'27'
	// <0x5956fe>|0x0ee|+0x067:'28'
	// <0x595765>|0x155|+0x06d:'29'
	// <0x5957d2>|0x1c2|+0x067:'30'
	// <0>
	// <0x595839>|0x229|+0x00f:'32'
	// <0x595848>|0x238|+0x00f:'33'
	// <0x595857>|0x247|+0x00f:'34'
	// <0x595866>|0x256|+0x00f:'35'
	// <0>
	// <0x595875>|0x265|+0x066:'37'
	// <0x5958db>|0x2cb|+0x066:'38'
	// <0x595941>|0x331|+0x075:'39'
	// <0>
	// <0x5959b6>|0x3a6|+0x075:'41'
	// <0x595a2b>|0x41b|+0x075:'42'
	// <0>
	// <0x595aa0>|0x490|+0x075:'44'
	// <0x595b15>|0x505|+0x075:'45'
	// <0x595b8a>|0x57a|+0x075:'46'
	// <0>
	// <0x595bff>|0x5ef|+0x075:'48'
	// <0x595c74>|0x664|+0x075:'49'
	// <0>
	// <0x595ce9>|0x6d9|+0x075:'51'
	// <0>
	// <0x595d5e>|0x74e|+0x087:'53'
	// <0>
	// <0x595de5>|0x7d5|+0x00c:'55'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::~weapon_user_animations_selector()
weapon_user_animations_selector::~weapon_user_animations_selector( )
{
	// LOCALS
	// ai::fsm_state* 				state<1>
	// ******

	// FUNCTION BODY
	// <0x5950ba>|0x00a|+0x011|[1]:'60'
	// <0x5950cb>|0x01b|+0x028:'61'
	// ******
}

// STATE[53.98%|PARTIAL]: control flow + all field reads match. Two divergences are
// compiler-inlining decisions (not source-steerable): (1) target inlines the first
// condition as `is_trying_to_sprint()` => `(actions_mask & 0x200) != 0` with a bool
// materialization (neg/sbb/neg); is_trying_to_sprint() is `/* no source */` in
// player_input.h (out of this unit's files), so we can't reproduce the bool round-trip
// without editing it. (2) target out-of-lines the `input().is_sprinting()` call while
// our base inlines the inline accessor (the inverse choice). See selector_remaining.md.
void weapon_user_animations_selector::tick( )
{
	if ( !( m_user->input( ).actions_mask & 0x200 ) )
		m_forced_not_to_sprint = false;
	else if ( !( m_user->input( ).is_sprinting( ) && is_weapon_in_idle( ) ) && current_state( ).id( ) == type_sprint )
		m_forced_not_to_sprint = true;

	m_logic.tick( );

	// FUNCTION BODY
	// <0x595489>|0x009|+0x02f:'66'	if ( !( m_user->input().actions_mask & 0x200 ) )
	// <0x5954b8>|0x038|+0x007:'68'	m_forced_not_to_sprint = false;
	// <0x5954bf>|0x03f|+0x04c:'70'	else if ( ... && current_state().id() == type_sprint )
	// <0x59550b>|0x08b|+0x007:'72'	m_forced_not_to_sprint = true;
	// <0x595512>|0x092|+0x008:'74'	m_logic.tick();
	// ******
}

// STATE[70.26%|PARTIAL]: structure matches (empty_stub ASSERT at L81, +20h/+27h
// field reads in consumers) but the target materializes the cast result through an
// extra `call <operator*>` (ICF-folds to `vec_begin`, `mov eax,[ecx]; ret`) taking
// `&(fsm_state* temp)` at L80 - a smart-pointer/deref idiom not yet identified, so our
// static_cast collapses it to a plain store. See
// docs/binary_matching/game_core/weapon_user_animations_selector_state_accessors.md.
player_logic_base_state& weapon_user_animations_selector::current_state( ) const
{
	player_logic_base_state* const result = static_cast< player_logic_base_state* >( m_logic.current_state( ) );
	ASSERT( UNKNOWN_EXPRESSION_T( result ) );
	return *result;

	// FUNCTION BODY
	// <0x594a39>|0x009|+0x01a:'80'	result = <operator*>( m_logic.current_state() )  [target: lea &temp; call vec_begin]
	// <0x594a53>|0x023|+0x00c:'81'	ASSERT( ... )  [empty_stub - matched]
	// <0x594a5f>|0x02f|+0x003:'82'	return *result
	// ******

	// claude@NOTE target current_state() asm (the divergence is the extra call at L80):
	// 0x09: mov ecx,[eax+10h]; mov [ebp-10h],ecx; mov edx,[ebp-10h]; mov [ebp-8],edx
	// 0x18: lea eax,[ebp-8]; call <operator*>(==vec_begin); mov [ebp-4],eax   <- MISSING in base
	// 0x23: mov byte[ebp-9],0; lea eax,[ebp-9]; call empty_stub               <- ASSERT, matched
	// 0x2f: mov eax,[ebp-4]; ret
}

// STATE[100%|DONE]
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> weapon_user_animations_selector::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	return current_state( ).selected_animations( buffer, weapon_parameters, is_third_view );
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::activate(survarium::base_player&, boost::function<void __cdecl(void)> const&, boost::function<void __cdecl(void)> const&)
void weapon_user_animations_selector::activate( base_player& user, boost::function<void()> const& sprint_start_callback, boost::function<void()> const& sprint_end_callback )
{
	// LOCALS
	// ai::fsm_state* 				i<1>
	// ******

	// CALL SITE INFO
	// <0x594fd9> -> void <unknown>(base_player&)
	// <0x59505c> -> void <unknown>(animation::reserved_channel_ids_enum, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::managed_resource_ptr const&, pcvoid const)
	// <0x595088> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x594f9a>|0x00a|+0x009:'101'
	// <0x594fa3>|0x013|+0x021|[1]:'102'
	// <0x594fc4>|0x034|+0x019:'103'
	// <0>
	// <0x594fdd>|0x04d|+0x00f:'105'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x594fec>|0x05c|+0x082:'113'
	// <0>
	// <0x59506e>|0x0de|+0x028:'115'
	// <0x595096>|0x106|+0x010:'116'
	// ******
}

// STATE[34.88%|PARTIAL]: set_initial_state(NULL) + the unsubscribe args/structure match.
// Two divergences: (1) L122 unsubscribe_animation_player resolves to base_player vtable
// +58h in base vs +54h in target - a one-slot delta from MSVC overloaded-virtual ordering.
// The CALL SITE INFO confirms the (reserved_channel_ids_enum, pcvoid) overload; this is a
// SOURCE-STEERABLE re-match opportunity - reordering base_player.h's two
// unsubscribe_animation_player overload declarations would shift the slot (it just lives in
// another header). (2) L124: target evaluates the unsubscribe_from_affect args (push 4,
// &m_leg_damaged_subscriber@0x18) THEN derefs via `call intrusive_ptr<...>::operator*`,
// while our base derefs first and resolves operator* to a different fold (`dummy::nonnull`).
// Same deref-idiom/arg-eval-order wall as current_state(); see
// weapon_user_animations_selector_state_accessors.md.
void weapon_user_animations_selector::deactivate( )
{
	m_logic.set_initial_state( NULL );
	m_user->unsubscribe_animation_player( animation::channel_id_on_animation_interval_end, this );
	( *m_user->damage_model( ) ).unsubscribe_from_affect( affects_type_leg_damage, &m_leg_damaged_subscriber );

	// FUNCTION BODY
	// <0x594e99>|0x009|+0x00a:'121'	m_logic.set_initial_state( NULL )
	// <0x594ea3>|0x013|+0x019:'122'	m_user->unsubscribe_animation_player( ..., this )   [base vtable+58 vs target+54]
	// <0>
	// <0x594ebc>|0x02c|+0x028:'124'	(*m_user->damage_model()).unsubscribe_from_affect( affects_type_leg_damage, &m_leg_damaged_subscriber )
	// ******
}

// STATE[100%|DONE]
bool weapon_user_animations_selector::is_ready_to_be_deactivated( ) const
{
	return current_state( ).is_ready_to_be_deactivated( );
}

// STATE[100%|DONE]
bool weapon_user_animations_selector::is_sprinting( ) const
{
	return current_state( ).id( ) == type_sprint;
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_user_animations_selector::serialize(vostok::network_core::udp_match_packet&) const
void weapon_user_animations_selector::serialize( network_core::udp_match_packet& packet ) const
{
	// LOCALS
	// u8 							state_id
	// bool 						found
	// ai::fsm_state const* 		current
	// ai::fsm_state const* 		i<1>
	// ******

	// CALL SITE INFO
	// <0x594e3d> -> void <unknown>(network_core::udp_match_packet&) const
	// ******

	// FUNCTION BODY
	// <0x594dc9>|0x009|+0x004:'139'
	// <0x594dcd>|0x00d|+0x004:'140'
	// <0x594dd1>|0x011|+0x009:'141'
	// <0x594dda>|0x01a|+0x025|[1]:'142'
	// <0x594dff>|0x03f|+0x008:'143'
	// <0x594e07>|0x047|+0x004:'144'
	// <0x594e0b>|0x04b|+0x002:'145'
	// <0>
	// <0x594e0d>|0x04d|+0x002:'147'
	// <0>
	// <0x594e0f>|0x04f|+0x00c:'149'
	// <0>
	// <0x594e1b>|0x05b|+0x00d:'151'
	// <0x594e28>|0x068|+0x017:'152'
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_user_animations_selector::deserialize(vostok::network_core::packet_reader&)
void weapon_user_animations_selector::deserialize( network_core::packet_reader& reader )
{
	// LOCALS
	// u8 							target_state_id
	// u8 							state_id
	// ai::fsm_state* 				current
	// ai::fsm_state* 				i<1>
	// ******

	// CALL SITE INFO
	// <0x594dac> -> void <unknown>(network_core::packet_reader&)
	// ******

	// FUNCTION BODY
	// <0x594d29>|0x009|+0x00b:'157'
	// <0x594d34>|0x014|+0x004:'158'
	// <0x594d38>|0x018|+0x007:'159'
	// <0x594d3f>|0x01f|+0x02a|[1]:'160'
	// <0x594d69>|0x049|+0x00c:'161'
	// <0x594d75>|0x055|+0x006:'162'
	// <0x594d7b>|0x05b|+0x002:'163'
	// <0>
	// <0x594d7d>|0x05d|+0x002:'165'
	// <0>
	// <0x594d7f>|0x05f|+0x00c:'167'
	// <0x594d8b>|0x06b|+0x00c:'168'
	// <0x594d97>|0x077|+0x017:'169'
	// ******
}

// STATE[71.5%|PARTIAL]: same divergence as current_state() - target materializes the
// inlined `m_logic.current_state()` + cast through TWO extra temps (frame 0x14 vs our
// 0x0C) the static_cast chain collapses. Field reads (+10h/+20h) and control flow match.
weapon_user_state_enum weapon_user_animations_selector::get_current_state_id( ) const
{
	return static_cast< player_logic_base_state* >( m_logic.current_state( ) )->id( );

	// FUNCTION BODY
	// <0x594a09>|0x009|+0x021:'174'	return static_cast<player_logic_base_state*>( m_logic.current_state() )->id()
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::sprint_predicate() const
bool weapon_user_animations_selector::sprint_predicate( ) const
{
	// CALL SITE INFO
	// <0x5953f0> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// <0x595433> -> player_input const& <unknown>() const
	// <0x595454> -> player_stamina& <unknown>()
	// ******

	return false;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x5953b9>|0x009|+0x0be:'190'
	// <0>
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::stand_predicate() const
bool weapon_user_animations_selector::stand_predicate( ) const
{
	// CALL SITE INFO
	// <0x59556b> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// <0x5955a4> -> player_input const& <unknown>() const
	// <0x5955d3> -> physics::bt_character_controller& <unknown>()
	// ******

	return false;

	// FUNCTION BODY
	// <0x595529>|0x009|+0x02a:'196'
	// <0>
	// <0x595553>|0x033|+0x007:'198'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x59555a>|0x03a|+0x09c:'205'
	// <0>
	// <0x5955f6>|0x0d6|+0x012:'207'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::set_animation_callback(char const*, void const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&)
void weapon_user_animations_selector::set_animation_callback( pcstr channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	// CALL SITE INFO
	// <0x594f7b> -> void <unknown>(pcstr, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::managed_resource_ptr const&, const u8, pcvoid const)
	// ******

	// FUNCTION BODY
	// <0x594f49>|0x009|+0x03c:'215'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::remove_animation_callback(char const*, void const*)
void weapon_user_animations_selector::remove_animation_callback( pcstr channel_id, pcvoid callback_uid )
{
	// CALL SITE INFO
	// <0x594950> -> void <unknown>(pcstr, pcvoid)
	// ******

	// FUNCTION BODY
	// <0x594937>|0x007|+0x01b:'220'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::set_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&)
void weapon_user_animations_selector::set_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	// CALL SITE INFO
	// <0x594f26> -> void <unknown>(animation::reserved_channel_ids_enum, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const&, pcvoid, resources::managed_resource_ptr const&, pcvoid const)
	// ******

	// FUNCTION BODY
	// <0x594ef9>|0x009|+0x037:'225'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::remove_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*)
void weapon_user_animations_selector::remove_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid )
{
	// CALL SITE INFO
	// <0x594920> -> void <unknown>(animation::reserved_channel_ids_enum, pcvoid)
	// ******

	// FUNCTION BODY
	// <0x594907>|0x007|+0x01b:'230'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::crouch_predicate() const
bool weapon_user_animations_selector::crouch_predicate( ) const
{
	// CALL SITE INFO
	// <0x594b79> -> player_input const& <unknown>() const
	// <0x594ba8> -> physics::bt_character_controller& <unknown>()
	// ******

	return false;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x594b59>|0x009|+0x070:'243'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::broken_legs_predicate() const
bool weapon_user_animations_selector::broken_legs_predicate( ) const
{
	// CALL SITE INFO
	// <0x594b1a> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// ******

	return false;

	// FUNCTION BODY
	// <0x594b09>|0x009|+0x03c:'248'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::jump_predicate() const
bool weapon_user_animations_selector::jump_predicate( ) const
{
	// LOCALS
	// bool 						result
	// ******

	// CALL SITE INFO
	// <0x5952c0> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// <0x5952fc> -> player_input const& <unknown>() const
	// <0x59532e> -> physics::bt_character_controller& <unknown>()
	// <0x59534d> -> player_stamina& <unknown>()
	// <0x595370> -> player_stamina& <unknown>()
	// ******

	return false;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x595289>|0x009|+0x11e:'261'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x5953a7>|0x127|+0x003:'274'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::is_weapon_in_idle() const
bool weapon_user_animations_selector::is_weapon_in_idle( ) const
{
	return false;

	// FUNCTION BODY
	// <0x5951f9>|0x009|+0x074:'279'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::is_weapon_firing() const
bool weapon_user_animations_selector::is_weapon_firing( ) const
{
	return false;

	// FUNCTION BODY
	// <0x5951a9>|0x009|+0x040:'284'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_user_animations_selector::is_weapon_toggling() const
bool weapon_user_animations_selector::is_weapon_toggling( ) const
{
	return false;

	// FUNCTION BODY
	// <0x595129>|0x009|+0x066:'289'
	// ******
}

// STATE[STUB]
// float survarium::weapon_user_animations_selector::look_time_factor() const
float weapon_user_animations_selector::look_time_factor( ) const
{
	// CALL SITE INFO
	// <0x594ad1> -> float <unknown>() const
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x594aa9>|0x009|+0x050:'301'
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
	float		time_scale
) const
{
	return 0.0f;

	// FUNCTION BODY
	// <0x594c99>|0x009|+0x041:'313'
	// <0x594cda>|0x04a|+0x00b:'314'
	// ******
}

// STATE[100%|DONE]
fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> weapon_user_animations_selector::look_time_calculator( ) const
{
	return look_time_functor( this, &weapon_user_animations_selector::look_time_factor_calculator );
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_user_animations_selector::on_interval_ended(vostok::animation::animation_callback_params&)
animation::callback_return_type_enum weapon_user_animations_selector::on_interval_ended( animation::animation_callback_params& params )
{
	// CONSTANTS
	// const weapon_user_animations_selector::on_interval_ended::__l9::<unnamed-tag> c_right_leg_interval_id = 0;
	// ******

	// FUNCTION BODY
	// <0x5949c9>|0x009|+0x00c:'324'
	// <0x5949d5>|0x015|+0x009:'325'
	// <0>
	// <0x5949de>|0x01e|+0x013|[1]:'327'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5949f1>|0x031|+0x002:'332'
	// ******
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::on_broken_limb_affect(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum)
void weapon_user_animations_selector::on_broken_limb_affect( pcstr bodypart, hit_affects_type_enum affect, affect_event_type_enum type )
{
	// FUNCTION BODY
	// <0x594969>|0x009|+0x023:'337'
	// <0>
	// <0x59498c>|0x02c|+0x023:'339'
	// <0>
	// <0x5949af>|0x04f|+0x00b:'341'
	// ******
}

// STATE[100%|DONE]
bool weapon_user_animations_selector::is_in_jump( ) const
{
	return current_state( ).id( ) == type_jump;
}

// STATE[STUB]
// void survarium::weapon_user_animations_selector::set_sprint_callbacks(boost::function<void __cdecl(void)> const&, boost::function<void __cdecl(void)> const&)
void weapon_user_animations_selector::set_sprint_callbacks( boost::function<void()> const& start_callback, boost::function<void()> const& end_callback )
{
	// LOCALS
	// player_logic_base_state* 	state
	// ******

	// FUNCTION BODY
	// <0x594c39>|0x009|+0x01b:'351'
	// <0x594c54>|0x024|+0x006:'352'
	// <0x594c5a>|0x02a|+0x00f:'353'
	// <0x594c69>|0x039|+0x010:'354'
	// <0>
	// <0x594c79>|0x049|+0x00f:'356'
	// <0x594c88>|0x058|+0x002:'357'
	// ******
}

} // namespace survarium
