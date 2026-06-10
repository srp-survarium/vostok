////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/game_core/player_logic_base_state.h>		// current_state().serialize/deserialize virtuals
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/player_input.h>
#include <vostok/game_core/weapon_core.h>

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

// STATE[68.66%|PARTIAL]
void weapon_user_animations_selector::tick( )
{
	if ( !m_user->input( ).is_trying_to_sprint( ) )
		m_forced_not_to_sprint = false;
	else if ( !( m_user->input( ).is_sprinting( ) && is_weapon_in_idle( ) ) && current_state( ).id( ) == type_sprint )
		m_forced_not_to_sprint = true;

	m_logic.tick( );

	// STRUCTURE DIFF: target 5 / base 5 stmts
	// BASE_ONLY |  78 | m_forced_not_to_sprint = false;
	// BASE_ONLY |  79 | else if ( !( m_user->input( ).is_sprinting( ) && is_weapon_in_idle( ) ) && current_state( ).id( ) == type_sprint )
	// TRGT_ONLY |  -- | L<else-if stmt 0x4c>
	// TRGT_ONLY |  -- | L<store-true stmt 0x7>
	// VERDICT: STRUCTURE MATCH - 5/5 stmts, the ONLY rows are aligner drift from one SIZE wall: target calls player_input::is_sprinting out-of-line (a player_input_inline.h COMDAT the LTCG kept) while base inlines its mask body; first-if + m_logic.tick() match after the is_trying_to_sprint( ) fill-in.
}

// STATE[80.21%|PARTIAL]
// claude@MATCH: the "deref idiom" is static_cast_checked - its non-DEBUG body is an
// inline cref-taking cast wrapper; the target's `call <op*>` IS the static_cast_checked
// instantiation kept out-of-line by LTCG (ICF-folded onto intrusive_ptr<...>::operator*,
// body `mov eax,[eax]; ret`), taking &(the cref-bound fsm_state* temp).
player_logic_base_state& weapon_user_animations_selector::current_state( ) const
{
	player_logic_base_state* const result = static_cast_checked< player_logic_base_state* >( m_logic.current_state( ) );
	ASSERT( UNKNOWN_EXPRESSION_T( result ) );
	return *result;

	// STRUCTURE DIFF: target 3 / base 3 stmts
	// SIZE -0x5 | 92 | player_logic_base_state* const result = static_cast_checked< player_logic_base_state* >( m_logic.current_state( ) );
	// VERDICT: STRUCTURE MATCH - frame/slots/ASSERT byte-identical to target; sole SIZE is the static_cast_checked instantiation called out-of-line in target vs inlined copy in base, LTCG inline-vs-call, non-steerable.
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

// STATE[34.91%|PARTIAL]: set_initial_state(NULL) + the unsubscribe_animation_player call now
// match. The L122 vtable-slot delta is FIXED: reordering base_player.h's two
// unsubscribe_animation_player overload declarations moved the (reserved_channel_ids_enum,
// pcvoid) overload to vtable slot +54h, so base now emits `mov eax,[edx+54h]; call eax` like
// the target (was +58h). Remaining (separate) divergence is L124: target evaluates the
// unsubscribe_from_affect args (push 4, &m_leg_damaged_subscriber@0x18) THEN derefs via
// `call intrusive_ptr<booby_trap_core,...>::operator*` (frame `sub esp,8`), while base derefs
// first and resolves operator* to a different fold (`dummy::nonnull`, frame `sub esp,10h`).
// Same deref-idiom/arg-eval-order wall as current_state(); see
// weapon_user_animations_selector_state_accessors.md.
void weapon_user_animations_selector::deactivate( )
{
	m_logic.set_initial_state( NULL );
	m_user->unsubscribe_animation_player( animation::channel_id_on_animation_interval_end, this );
	( *m_user->damage_model( ) ).unsubscribe_from_affect( affects_type_leg_damage, &m_leg_damaged_subscriber );

	// STRUCTURE DIFF: target 3 / base 3 stmts
	// SIZE +0x13 | 178 | ( *m_user->damage_model( ) ).unsubscribe_from_affect( affects_type_leg_damage, &m_leg_damaged_subscriber );
	// VERDICT: STRUCTURE MATCH - sole SIZE is the resource_ptr operator* out-of-line call + arg-eval order in target vs inlined deref in base (shared intrusive_ptr_inline.h, not steerable from this TU).
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

// STATE[81.83%|PARTIAL]: walks m_logic's state list to find the current state's index,
// appends that u8, then forwards serialize to the current state.
void weapon_user_animations_selector::serialize( network_core::udp_match_packet& packet ) const
{
	u8							state_id	= 0;
	bool						found		= false;
	ai::fsm_state const* const	current		= m_logic.current_state( );

	for ( ai::fsm_state const* i = m_logic.states( ).front( ); i; i = i->next, ++state_id )
	{
		if ( i == current )
		{
			found	= true;
			break;
		}
	}

	ASSERT( UNKNOWN_EXPRESSION_T( found ) );

	packet.append( state_id );
	static_cast_checked< player_logic_base_state const* >( current )->serialize( packet );

	// STRUCTURE DIFF: target 11 / base 11 stmts
	// SIZE +0x6 | 179 | ai::fsm_state const* const	current		= m_logic.current_state( );
	// SIZE -0x2 | 181 | for ( ai::fsm_state const* i = m_logic.states( ).front( ); i; i = i->next, ++state_id )
	// SIZE +0x7 | 192 | packet.append( state_id );
	// VERDICT: STRUCTURE MATCH - forward via static_cast_checked now byte-matches; residuals are the accessor ret-temp coalescing (target stores [this+10h] direct), states().front() call-vs-inline, and the packet append wall, LTCG, non-steerable.
}

// STATE[70.14%|PARTIAL]: reads the target state index, walks m_logic's state list to that
// index, promotes it to the initial state, then forwards deserialize.
void weapon_user_animations_selector::deserialize( network_core::packet_reader& reader )
{
	u8 const					target_state_id	= reader.r< bool >( );
	u8							state_id		= 0;
	ai::fsm_state*				current			= NULL;

	for ( ai::fsm_state* i = m_logic.states( ).front( ); i; i = i->next, ++state_id ) // sushi@TODO: same comma-in-for-tail doubt as serialize above
	{
		if ( state_id == target_state_id )
		{
			current	= i;
			break;
		}
	}

	ASSERT( UNKNOWN_EXPRESSION_T( current ) );

	m_logic.set_initial_state( current );
	static_cast_checked< player_logic_base_state* >( current )->deserialize( reader );

	// STRUCTURE DIFF: target 11 / base 11 stmts
	// SIZE +0x15 | 200 | u8 const					target_state_id	= reader.r< bool >( );
	// SIZE -0x7  | 204 | for ( ai::fsm_state* i = m_logic.states( ).front( ); i; i = i->next, ++state_id )
	// VERDICT: STRUCTURE MATCH - forward via static_cast_checked now byte-matches; residuals are packet_reader::r<bool> inline (base) vs call (target) - the network_core r<T> wall - and states().front() call-vs-inline, non-steerable.
}

// STATE[100%|DONE]: static_cast_checked reproduces the target's cref-bind + return temps.
weapon_user_state_enum weapon_user_animations_selector::get_current_state_id( ) const
{
	return static_cast_checked< player_logic_base_state* >( m_logic.current_state( ) )->id( );
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

// STATE[78.28%|PARTIAL]: body byte-correct (subscribe_animation_player(channel_id,
// animation_callback, callback_uid, managed_resource_ptr(), 0xff, NULL) at vtable+50h). The
// residual is the base_player vtable arg-count/this-load shape: base emits one fewer pushed
// arg and a `mov ecx,eax` this-setup vs the target's direct ecx. Rooted in base_player.h's
// virtual overload/vtable layout (owned by another matcher; same class as the deactivate
// slot fix), not steerable from this TU.
void weapon_user_animations_selector::set_animation_callback( pcstr channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, resources::managed_resource_ptr( ), 0xff, NULL );

	// STRUCTURE DIFF: target 1 / base 1 stmts
	// SIZE -0x3 | 293 | m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, resources::managed_resource_ptr( ), 0xff, NULL );
	// VERDICT: STRUCTURE MATCH - sole SIZE is the base_player vtable arg/this-load shape (base_player.h owned elsewhere), non-steerable from this TU.
}

// STATE[100%|DONE]
void weapon_user_animations_selector::remove_animation_callback( pcstr channel_id, pcvoid callback_uid )
{
	m_user->unsubscribe_animation_player( channel_id, callback_uid );
}

// STATE[84.79%|PARTIAL]: same base_player vtable shape residual as the pcstr overload
// (subscribe_animation_player(channel_id, animation_callback, callback_uid,
// managed_resource_ptr(), NULL) at vtable+4Ch).
void weapon_user_animations_selector::set_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, resources::managed_resource_ptr( ), NULL );

	// STRUCTURE DIFF: target 1 / base 1 stmts
	// SIZE -0x3 | 307 | m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, resources::managed_resource_ptr( ), NULL );
	// VERDICT: STRUCTURE MATCH - same base_player vtable arg/this-load shape residual as the pcstr overload, non-steerable from this TU.
}

// STATE[100%|DONE]
void weapon_user_animations_selector::remove_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid )
{
	m_user->unsubscribe_animation_player( channel_id, callback_uid );
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

// STATE[76.76%|PARTIAL]: body byte-correct (m_user@+44h read twice, virtual damage_model
// @vtable+0Ch, broken_legs_count = byte[+339h]+byte[+338h], == 2 via xor/cmp/sete). The
// sole divergence is the intrusive_ptr operator* out-of-lining: target emits a real
// `call intrusive_ptr<booby_trap_core,...>::operator*` (a `mov eax,[eax]; ret` COMDAT with
// NO assert), while base inlines operator* (our intrusive_ptr_inline.h operator* carries an
// ASSERT, folded to `dummy::nonnull`). Same deref-idiom wall as current_state/deactivate;
// rooted in shared intrusive_ptr_inline.h (not steerable from this TU). See _state_accessors.md.
bool weapon_user_animations_selector::broken_legs_predicate( ) const
{
	return ( *m_user->damage_model( ) ).broken_legs_count( ) == 2;

	// STRUCTURE DIFF: target 1 / base 1 stmts
	// SIZE +0xf | 398 | return ( *m_user->damage_model( ) ).broken_legs_count( ) == 2;
	// VERDICT: STRUCTURE MATCH - sole SIZE is the resource_ptr operator* out-of-line (no-assert COMDAT) in target vs inlined deref in base (shared intrusive_ptr_inline.h), non-steerable from this TU.
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

// STATE[41.56%|PARTIAL]: body shape correct (cast *current_active_object() -> weapon_core,
// read the inlined predicate field at +48Ch). WALL: target's current_active_object() returns
// `intrusive_ptr<inventory_item,...>` (a by-value temp copy-ctor'd at [ebp-8], operator*
// out-of-lined), while base's interactive_object_ptr is `intrusive_ptr<game_world_object,...>`
// (different root) AND base inlines the copy-ctor. Both stem from base_player.h /
// interactive_object.h typedef + the intrusive_ptr deref idiom, neither steerable here.
bool weapon_user_animations_selector::is_weapon_in_idle( ) const
{
	return static_cast< weapon_core const& >( *m_user->current_active_object( ) ).is_idle( );

	// STRUCTURE DIFF: target 1 / base 1 stmts
	// SIZE +0x15 | 457 | return static_cast< weapon_core const& >( *m_user->current_active_object( ) ).is_idle( );
	// VERDICT: STRUCTURE MATCH - wall: target copies an intrusive_ptr<inventory_item> temp via an out-of-line copy-ctor (base inlines the pin logic over intrusive_ptr<game_world_object> - base_player.h/interactive_object.h typedef root), and target's inlined is_idle( ) reads THREE flags (+492h || (+488h && !+48Ch)) vs base's single +492h read (weapon_core.h is_idle body, owned elsewhere).
}

// STATE[8.96%|PARTIAL]: same wall as is_weapon_in_idle (current_active_object intrusive_ptr
// root type + operator* out-of-lining, base_player/interactive_object owned elsewhere).
bool weapon_user_animations_selector::is_weapon_firing( ) const
{
	return static_cast< weapon_core const& >( *m_user->current_active_object( ) ).is_firing( );

	// STRUCTURE DIFF: target 1 / base 1 stmts
	// SIZE +0x49 | 468 | return static_cast< weapon_core const& >( *m_user->current_active_object( ) ).is_firing( );
	// VERDICT: STRUCTURE MATCH - same current_active_object intrusive_ptr root/copy-ctor wall as is_weapon_in_idle (base_player.h/interactive_object.h owned elsewhere).
}

// STATE[34.89%|PARTIAL]: same wall as is_weapon_in_idle.
bool weapon_user_animations_selector::is_weapon_toggling( ) const
{
	return static_cast< weapon_core const& >( *m_user->current_active_object( ) ).is_toggling( );

	// STRUCTURE DIFF: target 1 / base 1 stmts
	// SIZE +0x23 | 478 | return static_cast< weapon_core const& >( *m_user->current_active_object( ) ).is_toggling( );
	// VERDICT: STRUCTURE MATCH - same current_active_object intrusive_ptr root/copy-ctor wall as is_weapon_in_idle (base_player.h/interactive_object.h owned elsewhere).
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

// STATE[100%|DONE]
animation::callback_return_type_enum weapon_user_animations_selector::on_interval_ended( animation::animation_callback_params& params )
{
	enum { c_right_leg_interval_id };

	ASSERT( UNKNOWN_EXPRESSION_T( &params ) );
	if ( params.animation_user_data == 1 )
		m_right_leg_is_supporting = params.animation_interval_id != c_right_leg_interval_id;

	return animation::callback_return_type_call_me_again;
}

// STATE[86.38%|PARTIAL]: all 5 statements match (was 20.12% / 2 stmts). First two ASSERTs are
// byte-perfect: L337 = `ASSERT_T_U( bodypart, type )` (2-arg eater, no `push 0` - the assert_type
// slot holds a runtime value), L339 = `ASSERT_CMP_U( affect, ==, 4 )`. Residual is L341 only:
// `force_animation_selection()` is declared inline in base_player.h, so our build inlines
// `m_user->m_force_animation_selection = true` (mov byte[ecx+118h],1) while the target kept the
// call out-of-line (`call base_player::force_animation_selection`). Same whole-program inline-vs-call
// LTCG residual as player_logic_sprint_state; base_player.h is owned by another unit (not editable here).
void weapon_user_animations_selector::on_broken_limb_affect( pcstr bodypart, hit_affects_type_enum affect, affect_event_type_enum type )
{
	ASSERT_T_U( bodypart, type );
	ASSERT_CMP_U( affect, ==, 4 );
	m_user->force_animation_selection( );

	// STRUCTURE DIFF: target 3 / base 3 stmts
	// SIZE +0x8 | 548 | m_user->force_animation_selection( );
	// VERDICT: STRUCTURE MATCH - both ASSERT eaters byte-perfect; sole SIZE is force_animation_selection kept out-of-line in target vs the base_player.h inline body inlined in base, LTCG inline-vs-call (base_player.h owned elsewhere).
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
