////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_USER_ANIMATIONS_SELECTOR_H_INCLUDED
#define WEAPON_USER_ANIMATIONS_SELECTOR_H_INCLUDED

#include <vostok/ai/fsm.h>
#include <vostok/animation/animation_player.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_expression.h>

#include <vostok/game_core/weapon_user_animations_container.h>
#include <vostok/game_core/weapon_user_state_enum.h>
#include <vostok/game_core/damage_model.h> // sushi@NOTE: For affect_subscriber

namespace vostok {
namespace network_core {
	class udp_match_packet;
	class packet_reader;
}
namespace animation {
	struct animation_callback_params;
}
}

namespace survarium {

struct base_player;
class player_logic_base_state;
struct weapon_animation_parameters;

class weapon_user_animations_selector : private boost::noncopyable {
public:
	typedef boost::function< void( resources::managed_resource_ptr const&, pcstr ) > animation_callback_type;
	typedef std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > selected_animations_result_type;
	enum { c_move_animation_user_data = 1 };

	explicit				weapon_user_animations_selector	( );
							~weapon_user_animations_selector( );

public:
			void			set_animations					( weapon_user_animations_container_ptr const& value ) { m_animations = value; }
	inline	weapon_user_animations_container const&
							animations						( ) const { return *m_animations; }

public:
			void			set_animation_callback			(
								pcstr						channel_id,
								pcvoid						callback_uid,
								boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback
							);
			void			set_animation_callback			(
								animation::reserved_channel_ids_enum	channel_id,
								pcvoid									callback_uid,
								boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback
							);

			void			remove_animation_callback		( pcstr channel_id, pcvoid callback_uid );
			void			remove_animation_callback		( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid );

public:
			void			serialize						( network_core::udp_match_packet& packet ) const;
			void			deserialize						( network_core::packet_reader& reader );

public:
			bool			is_ready_to_be_deactivated		( ) const;
			bool			is_sprinting					( ) const;
			bool			is_in_jump						( ) const;

			selected_animations_result_type
							selected_animations				( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, const bool is_third_view ) const;

			void			activate						(
								base_player& user,
								boost::function<void()> const& sprint_start_callback,
								boost::function<void()> const& sprint_end_callback
							);
			void			deactivate						( );

	inline	base_player&	user							( ) const { ASSERT( m_user ); return *m_user; }
			void			tick							( );

public:
			float				look_time_factor			( ) const;
			fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) >
							look_time_calculator		( ) const;

public:
			weapon_user_state_enum		get_current_state_id			( ) const;
			bool						sprint_predicate				( ) const;

	inline	ai::fsm&							logic							( ) { return m_logic; }

	inline	bool			is_right_leg_supporting			( ) const { return m_right_leg_is_supporting; }

	inline	void			set_player_logic_initial_state	( player_logic_base_state* value ) { m_player_logic_initial_state = value; }

private:
	inline	void			set_forced_not_to_sprint		( const bool arg_0 ) { /* no source */ }

	// claude@MATCH: target mangling `ABE` -> private const.
			player_logic_base_state&		current_state	( ) const;

private:
			bool			stand_predicate					( ) const;
			bool			crouch_predicate				( ) const;
			bool			broken_legs_predicate			( ) const;
			bool			jump_predicate					( ) const;

private:
			bool			is_weapon_in_idle				( ) const;
			bool			is_weapon_firing				( ) const;
			bool			is_weapon_toggling				( ) const;

			float			look_time_factor_calculator		(
								const float		animation_length,
								const float		animation_time_before_time_scale_starts,
								const u32			time_scale_start_time_in_ms,
								const u32			current_time_in_ms,
								const u32			target_time_in_ms,
								const float		time_scale
							) const;

private:
			animation::callback_return_type_enum
							on_interval_ended				( animation::animation_callback_params& params );

			void			on_broken_limb_affect			( pcstr bodypart, const hit_affects_type_enum affect, const affect_event_type_enum type );
			void			set_sprint_callbacks			( boost::function<void()> const& start_callback, boost::function<void()> const& end_callback );


private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	ai::fsm									m_logic;
	/* 0x0018 */	affect_subscriber						m_leg_damaged_subscriber;
	/* 0x0040 */	weapon_user_animations_container_ptr	m_animations;
	/* 0x0044 */	base_player*							m_user;
	/* 0x0048 */	player_logic_base_state*				m_player_logic_initial_state;
	/* 0x004c */	bool									m_forced_not_to_sprint;
	/* 0x004d */	bool									m_right_leg_is_supporting;
}; // class weapon_user_animations_selector

STATIC_SIZE_ASSERT(weapon_user_animations_selector, 0x50);

} // namespace survarium

#endif // #ifndef WEAPON_USER_ANIMATIONS_SELECTOR_H_INCLUDED
