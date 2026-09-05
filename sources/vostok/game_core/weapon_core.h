// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_CORE_H_INCLUDED
#define WEAPON_CORE_H_INCLUDED

#include <vostok/game_core/inventory_item.h>

#include <vostok/game_core/weapon_ammunition.h>
#include <vostok/game_core/weapon_core_base_state.h>
#include <vostok/game_core/weapon_targets.h>
#include <vostok/game_core/weapon_dispersion_params.h>
#include <vostok/game_core/weapon_recoil_params.h>
#include <vostok/game_core/weapon_user_animations_selector.h>

#include <vostok/game_core/breath_vibration_calculator.h>
#include <vostok/game_core/recoil_calculator.h>
#include <vostok/game_core/dispersion_calculator.h>

#include <vostok/game_core/hand_to_weapon_ik_processor.h>
#include <vostok/game_core/legs_ik_processor.h>

#include <vostok/game_core/normal_random.h>

#include <vostok/game_core/ammo_id_enum.h>
#include <vostok/game_core/profile_slot_enum.h>

#include <vostok/math_randoms_generator.h>

namespace vostok {
namespace ai {
	class fsm;
}
}

namespace survarium {

struct hit_initiator;
struct hit_receiver;
struct base_player;
class bullet_manager;
struct weapon_ammo_info;


class weapon_core : public inventory_item {
public:
	typedef weapon_recoil_params recoil_params;
	typedef weapon_dispersion_params dispersion_params;

			explicit							weapon_core						( );
	virtual										~weapon_core					( );

			void								set_skeleton					( animation::skeleton_ptr const& skeleton );

	inline	void								set_recoil_params				( recoil_params const& params )	{ m_recoil_params = params;	}
	inline	recoil_params const&					get_recoil_params				( ) const								{ return m_recoil_params;	}

	inline	void								set_dispersion_params			( dispersion_params const& params )	{ m_dispersion_params = params;	}
	inline	dispersion_params const&				get_dispersion_params			( ) const								{ return m_dispersion_params;	}

	inline	dispersion_calculator&				get_dispersion_calculator		( )												{ return m_dispersion_calculator;	}
	inline	dispersion_calculator const&		get_dispersion_calculator		( ) const										{ return m_dispersion_calculator;	}

			float								get_dispersion					( ) const;

			void								set_magazine_capacity			( u16 magazine_capacity );
	inline	u16									get_magazine_capacity			( ) const	{ return m_magazine_capacity; }

	inline	weapon_ammunition_ptr				ammunition						( ) const										{ return m_ammunition;				}
			void								set_ammunition					( weapon_ammunition_ptr const& ammunition_to_set );

			u16									ammo_in_magazine				( ) const;
	inline	u16									ammo_in_weapon					( ) const { return m_ammo_in_magazine + ( m_is_round_chambered != false ); }

			u16									maximum_ammo_in_weapon			( ) const;

	inline	bool								ready_to_fire					( ) const { return m_ready_for_fire; }

			bool								is_ready_to_shoot				( ) const;
	inline	bool								is_firing						( ) const { return m_is_firing; }
	inline	bool								is_toggling						( ) const { return m_is_toggling || !m_is_shown; }

			bool								ready_to_reload					( ) const;

	inline	float4x4 const&						get_bullet_transform			( ) const { return m_fire_bullet_transform; }
	inline	weapon_targets						get_target						( ) const { return m_target; }

	inline	void								set_fire_queue_type				( const u8 value ) { m_fire_queue_type = value; }
	inline	u8									get_fire_queue_type				( ) const { return m_fire_queue_type; }

	inline	u16									get_bullets_in_queue			( ) const { return m_bullets_in_queue; }
			u16									fire_queue_length				( ) const;

	inline	float4x4							get_transform					( ) const { return m_transform; }
	virtual	float4x4							transform						( ) const override { return m_transform; }
	virtual	void								set_transform					( float4x4 const& transform ) { m_transform = transform; }

	inline	hit_initiator const*				hit_initiator_holder			( ) const { return m_initiator_holder; }
	// sushi@TODO: get_skeleton and user_animations use ownership-based models; verify an original consumer.
	inline	animation::skeleton_ptr				get_skeleton					( ) const { return m_skeleton; }

	virtual	void								set_next_fire_queue_type		( );
	virtual	void								set_next_ammo_type				( );

	virtual	void								on_ammo_empty					( ) { }
	virtual	void								set_inventory					( inventory* inv, profile_slot_enum slot ) override;

			profile_slot_enum					get_ammo_slot					( ammo_id_enum slot_id );
			void								get_ammo_info					( weapon_ammo_info& info );

			fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) >
												backward_recoil_time_calculator		( );
			fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) >
												horizontal_recoil_time_calculator	( );
			fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) >
												vertical_recoil_time_calculator		( );

			weapon_user_animations_selector&		user_animations_selector		( )	{ return m_user_animations_selector; }
	inline	weapon_user_animations_selector const&	user_animations_selector		( ) const { return m_user_animations_selector; }

	virtual	animation::mixing::expression		selected_animations				( mutable_buffer& buffer, const bool is_third_view ) const override;

	virtual	void								set_target						( const weapon_targets target );

	virtual	void								tick							( ) override;

			void								instant_show					( );
			void								instant_hide					( );
			void								instant_reload					( );
			void								instant_chamber_a_round			( );
	virtual	void								instant_aim_start				( );
	virtual	void								instant_aim_end					( );
			void								instant_idle_start				( );
			void								instant_idle_end				( );
			void								instant_toggle_start			( );
			void								instant_toggle_end				( );
			void								instant_fire					( const u32 current_time_in_ms );

			void								reload_one_round				( );
	virtual	void								set_fire_bullet_transform		( float4x4 const& fire_bullet_transform );
	virtual	void								on_reload_started				( );
	virtual	bool								is_ready_to_be_deactivated		( ) const override;

	virtual	void								activate						( base_player& user, engine& engine ) override;
	virtual	void								deactivate						( ) override;

	virtual	bool								can_hold_breath					( ) const { return m_aimed; }
			void								reset_fire_queue				( );

			bool								is_aimed						( ) const;
	// claude@MATCH: body proven by weapon_user_animations_selector::is_weapon_in_idle target asm
	// (m_is_idle || (m_aimed && !m_is_firing), with the [+0x492]/[+0x488]/[+0x48c] field reads).
	inline	bool								is_idle							( ) const { return m_is_idle || ( m_aimed && !m_is_firing ); }

			void								unload_chambered_round			( );
			void								unload_ammo						( );

			void								load_magazine					( );
			void								chamber_a_round					( );

	// claude@MATCH: weapon_core_cook::load_weapon_parameters inlines these two setters
	// WITH an empty_stub (compiled-out ASSERT) before the member store.
	inline	void								set_bullet_damage				( float bullet_damage ) { ASSERT( UNKNOWN_EXPRESSION_T( bullet_damage >= 0.f ) ); m_bullet_damage = bullet_damage; }
	inline	float								get_bullet_damage				( ) const { return m_bullet_damage; }
	inline	void								set_bullet_pierce				( float bullet_pierce ) { ASSERT( UNKNOWN_EXPRESSION_T( bullet_pierce >= 0.f ) ); m_bullet_pierce = bullet_pierce; }
	inline	float								get_bullet_pierce				( ) const { return m_bullet_pierce; }
	inline	void								set_aim_fov_factor				( const float aim_fov_factor ) { m_aim_fov_factor = aim_fov_factor; }
	inline	float								aim_fov_factor					( ) const { return m_aim_fov_factor; }
	inline	void								set_aim_near_plane_factor		( const float aim_near_plane_factor ) { m_aim_near_plane_factor = aim_near_plane_factor; }
	inline	float								aim_near_plane_factor			( ) const { return m_aim_near_plane_factor; }

			profile_slot_enum					ammo_slot						( );

	inline	void								set_weapon_fire_queue_types		( pbyte weapon_fire_queue_types, u8 count ) { m_weapon_fire_queue_types = weapon_fire_queue_types; m_weapon_fire_queue_types_count = count; }

	inline	void									set_user_animations				( weapon_user_animations_container_ptr const& user_animations ) { m_user_animations_selector.set_animations( user_animations ); }
	inline	weapon_user_animations_container const&	user_animations					( ) const { return m_user_animations_selector.animations( ); }

	inline	base_player*						get_user						( ) const { return m_user; }
			bool								is_double_handed				( ) const;

	virtual	weapon_core*						cast_weapon_core				( ) override { return this; }
	virtual	weapon_core const*					cast_weapon_core				( ) const override { return this; }

			bool								could_be_used					( base_player const& user ) const;
			bool								could_be_aimed					( base_player const& user ) const;

			void								set_animation_callback			( pcstr channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback );
			void								set_animation_callback			( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback );

			void								remove_animation_callback		( pcstr channel_id, pcvoid callback_uid );
			void								remove_animation_callback		( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid );

	inline	bool								is_third_view					( ) const { return m_is_third_view; }
	inline	bool								has_chamber_a_round_state		( ) const { return m_is_there_chamber_a_round_state; }
			bool								round_is_chambered				( ) const;
	inline	bool								chamber_a_round_on_reload		( ) const { return m_chamber_a_round_on_reload; }
	inline	void								load_ammo_on_next_activate		( ) { m_load_ammo_on_next_activate = true; }

	inline	bool								deserializing					( ) const { return m_deserializing; }
	inline	bool								is_active						( ) const { return m_logic->current_state( ) != NULL; }

private:
	// claude@MATCH: target mangles target_predicate ABE (private), not QBE.
			bool								target_predicate						( const weapon_targets target ) const { return m_target == target; }
			bool								target_and_animation_ended_predicate	( const weapon_targets target ) const;
			bool								instant_idle_predicate					( ) const;

private:
			bool								must_chamber_a_round_predicate							( ) const;
			bool								must_chamber_a_round_aimed_predicate					( ) const;
			bool								must_chamber_a_round_and_animation_ended_predicate		( ) const;
			bool								must_chamber_a_round_aimed_and_animation_ended_predicate( ) const;

	// claude@MATCH: target mangles these AAE (private), not QAE.
			float3								get_dispersed_bullet_dir		( );

			void								update_recoil					( u32 current_time_in_ms, const float time_scale );
			void								update_dispersion				( const bool is_moving, u32 current_time_in_ms );
			void								update_breath_vibration			( const bool is_holding_breath, u32 current_time_in_ms, const float time_scale );

			animation::callback_return_type_enum	on_animation_ik_interval		( animation::animation_callback_params& params );
			animation::callback_return_type_enum	on_sprint_animation_ended		( animation::animation_callback_params& params );
			animation::callback_return_type_enum	fake_callback					( animation::animation_callback_params& params ) { return animation::callback_return_type_call_me_again; }

private:
	virtual	void								on_player_model_added			( ) override;
	virtual	void								on_player_model_removed			( ) override;

	// claude@MATCH: target mangles update_bones_matrices/serialize/deserialize
	// EAE/EBE/EAE (private virtual), not UAE/UBE.
	virtual	void								update_bones_matrices			(
													animation::skeleton_ptr const&		user_skeleton,
													float4x4* const						user_matrices,
													const u32									user_matrices_count,
													const u32									current_time_in_ms,
													float4x4&							character_head_transform,
													float4x4&							character_transform,
													animation::animation_player const&	user_animation_player
												) override;

	virtual	void								serialize						( network_core::udp_match_packet& packet, u32 client_offset ) const override;
	virtual	void								deserialize						( network_core::packet_reader& reader ) override;

	virtual	bool								is_sprinting					( ) const override;
private:

	virtual	void								on_before_fire					( ) { }
	virtual	void								on_after_fire					( ) { }
	virtual	void								on_reload						( ) { }
	virtual	void								on_chamber_a_round				( ) { }
	virtual	void								on_show							( ) { }
	virtual	void								on_hide							( ) { }
	virtual	void								on_unload_chambered_round		( ) { }

private:
	// claude@MATCH: target mangles the pointer params QBV/QAV (T* const) - keep the
	// top-level const on every pointer.
	virtual	void								on_skeleton_matrices_changed	(
													const u32					current_time_in_ms,
													float4x4 const&		weapon_transform,
													float4x4 const* const	weapon_matrices_begin,
													float4x4 const* const	weapon_matrices_end,
													float4x4 const&		user_transform,
													float4x4* const		user_matrices_begin,
													float4x4* const		user_matrices_end,
													float4x4 const&		user_weapon_transform
												);

	virtual	void								process_finger_correction		( const u32 current_time_in_ms, float4x4* const user_matrices );

	// claude@MATCH: target mangles this ABE (private const), not QBE.
			animation::mixing::expression		get_weapon_and_hands_animation_expression(
													mutable_buffer&						buffer,
													const bool								is_third_view,
													const weapon_user_state_enum				weapon_user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const;
			animation::body_part_masks_enum		get_body_part_mask_for_user		( ) const;

	// weapon_core_cook initializes the target's private state bundle directly.
	friend class weapon_core_cook;
			void								initialize_weapon_logic			(
													weapon_core_base_state_ptr const&	inactive_state,
													weapon_core_base_state_ptr const&	show_state,
													weapon_core_base_state_ptr const&	hide_state,
													weapon_core_base_state_ptr const&	idle_state,
													weapon_core_base_state_ptr const&	reload_state,
													weapon_core_base_state_ptr const&	fire_state,
													weapon_core_base_state_ptr const&	aim_state,
													weapon_core_base_state_ptr const&	aim_fire_state,
													weapon_core_base_state_ptr const&	chamber_a_round_state,
													weapon_core_base_state_ptr const&	chamber_a_round_aimed_state
												);

	inline	weapon_core_base_state&				current_base_state				( ) const { return *static_cast_checked< weapon_core_base_state* >( m_logic->current_state( ) ); }

			float								computed_backward_recoil_time	(
													const float		animation_length,
													const float		animation_time_before_time_scale_starts,
													const u32			time_scale_start_time_in_ms,
													const u32			current_time_in_ms,
													const u32			target_time_in_ms,
													const float		time_scale
												);

			float								computed_horizontal_recoil_time	(
													const float		animation_length,
													const float		animation_time_before_time_scale_starts,
													const u32			time_scale_start_time_in_ms,
													const u32			current_time_in_ms,
													const u32			target_time_in_ms,
													const float		time_scale
												);

			float								computed_vertical_recoil_time	(
													const float		animation_length,
													const float		animation_time_before_time_scale_starts,
													const u32			time_scale_start_time_in_ms,
													const u32			current_time_in_ms,
													const u32			target_time_in_ms,
													const float		time_scale
												);

	inline	float								backward_recoil_value			( ) const;
			float								horizontal_recoil_value			( ) const;
			float								vertical_recoil_value			( ) const;

private:
			bool								is_trying_to_aim				( ) const;
			bool								is_not_trying_to_aim_predicate	( ) const;
			bool								can_and_must_reload_predicate	( ) const;
			bool								can_and_must_reload_and_animation_ended_predicate( ) const;

	// claude@MATCH: target mangles load_ammo AAE (private), not QAE.
			void								load_ammo						( );

private:
			animation::callback_return_type_enum
												on_hand_ik_event				( animation::animation_callback_params& params, const hand_to_weapon_ik_processor::hands_enum hand );
protected:
	virtual	void								on_user_sprint					( bool user_is_sprinting );
public:
private:
	/* 0x0000 */	/* inventory_item */
	/* 0x0118 */	float4x4							m_fire_bullet_transform;
	/* 0x0158 */	float4x4							m_transform;
	/* 0x0198 */	hand_to_weapon_ik_processor			m_hand_ik_processor;
	/* 0x01d8 */	legs_ik_processor					m_legs_ik_processor;
	/* 0x0278 */	weapon_user_animations_selector		m_user_animations_selector;
	/* 0x02c8 */	recoil_calculator					m_recoil_calculator;
	/* 0x0328 */	dispersion_calculator				m_dispersion_calculator;
	/* 0x0374 */	breath_vibration_calculator			m_breath_vibration_calculator;
	/* 0x03b0 */	recoil_params						m_recoil_params;
	/* 0x03e4 */	dispersion_params					m_dispersion_params;
	/* 0x0404 */	weapon_ammunition_ptr				m_ammunition;
	/* 0x0408 */	animation::skeleton_ptr				m_skeleton;
	/* 0x040c */	hit_initiator*						m_initiator_holder;
	/* 0x0410 */	hit_receiver const*					m_receiver_holder;
	/* 0x0414 */	ai::fsm*							m_logic;
	typedef fixed_vector< weapon_core_base_state_ptr, 10 > logic_states_type;
	/* 0x0418 */	logic_states_type					m_logic_states;
	/* 0x0448 */	bullet_manager*						m_bullet_manager;
protected:
	/* 0x044c */	base_player*						m_user;
private:
	/* 0x0450 */	math::random32						m_random;
	/* 0x0454 */	pbyte								m_weapon_fire_queue_types;
	/* 0x0458 */	normal_random						m_normal_random;
	/* 0x045c */	float								m_bullet_damage;
	/* 0x0460 */	float								m_bullet_pierce;
	/* 0x0464 */	float								m_aim_fov_factor;
	/* 0x0468 */	float								m_aim_near_plane_factor;
	/* 0x046c */	weapon_targets						m_target;
	/* 0x0470 */	u32									m_old_actions_mask;
	/* 0x0474 */	u32									m_last_tick_time_in_ms;
	/* 0x0478 */	u16									m_magazine_capacity;
	/* 0x047a */	u16									m_ammo_in_magazine;
	/* 0x047c */	u16									m_bullets_in_queue;
	/* 0x047e */	u8									m_fire_queue_type;
	/* 0x0480 */	profile_slot_enum					m_ammo_slot;
	/* 0x0484 */	u8									m_weapon_id;
	/* 0x0485 */	u8									m_weapon_fire_queue_types_count;
	/* 0x0486 */	mutable bool						m_is_third_view;
	/* 0x0487 */	bool								m_is_shown;
	/* 0x0488 */	bool								m_aimed;
	/* 0x0489 */	bool								m_ready_for_fire;
	/* 0x048a */	bool								m_is_double_handed;
	/* 0x048b */	bool								m_is_in_sprint_transition;
	/* 0x048c */	bool								m_is_firing;
	/* 0x048d */	bool								m_is_there_chamber_a_round_state;
	/* 0x048e */	bool								m_is_round_chambered;
	/* 0x048f */	bool								m_chamber_a_round_on_reload;
	/* 0x0490 */	bool								m_load_ammo_on_next_activate;
	/* 0x0491 */	bool								m_aiming_state_transition;
	/* 0x0492 */	bool								m_is_idle;
	/* 0x0493 */	bool								m_deserializing;
	/* 0x0494 */	bool								m_is_toggling;

}; // class weapon_core

STATIC_SIZE_ASSERT(weapon_core, 0x498);

typedef resources::resource_ptr< weapon_core, resources::unmanaged_intrusive_base > weapon_core_ptr;

} // namespace survarium

#endif // #ifndef WEAPON_CORE_H_INCLUDED
