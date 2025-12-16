////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef OXYGEN_TANK_H_INCLUDED
#define OXYGEN_TANK_H_INCLUDED

#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/damage_protector.h>

namespace survarium {

class oxygen_tank : public inventory_item {
public:
	virtual	void								action						( bool key_down ) override;
	virtual	bool								get_item_props				( inventory_item_props& props ) override;

												oxygen_tank					( );
	virtual										~oxygen_tank				( );

			void								load						( configs::binary_config_value config );

	inline	bool								empty						( ) const { /* no source */ }

			void								set_active					( bool bactive );

			void								active_tick					( u32 frame_time_ms );

			float								reduce_damage				(
													pcstr		body_part_name,
													pcstr		damage_type,
													float		amount,
													float		armor_piercing
												);

	// STATE[STUB]
	virtual	void								activate					( base_player& user, engine& engine ) override { }
	// STATE[STUB]
	virtual	void								deactivate					( ) override { }
	// STATE[STUB]
	virtual	float4x4							transform					( ) const override { VOSTOK_UNREACHABLE_CODE( ); }

	virtual	void								tick						( ) override { /* no source */ }

	virtual	bool								is_ready_to_be_deactivated	( ) const override { /* no source */ }
	// STATE[STUB]
	virtual	animation::mixing::expression		selected_animations			( mutable_buffer& buffer, bool is_third_view ) const override { VOSTOK_UNREACHABLE_CODE( ); }

	virtual	void								on_player_model_added		( ) override { /* no source */ }
	virtual	void								on_player_model_removed		( ) override { /* no source */ }
	// STATE[STUB]
	virtual	void								update_bones_matrices		(
													animation::skeleton_ptr const&		user_skeleton,
													float4x4*							user_matrices,
													u32									user_matrices_count,
													u32									current_time_in_ms,
													float4x4&							character_head_transform,
													float4x4&							character_transform,
													animation::animation_player const&	animation_player
												) override { /* <0xcc810> */ }
	// STATE[STUB]
	virtual	void								serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const override { /* <0xccd90> */ }
	// STATE[STUB]
	virtual	void								deserialize					( network_core::packet_reader& reader ) override { /* <0xccdb0> */ }

	virtual	bool								is_sprinting				( ) const override { /* no source */ }

private:
	struct item_influence {
	public:
		inline	item_influence	( ) { /* no source */ }
		inline	~item_influence	( ) { /* no source */ }


	public:
		/* 0x0000 */	damage_protector	protector;
		/* 0x0050 */	char				body_part_name[16];
		/* 0x0060 */	char				hit_type[16];
		/* 0x0070 */	float				hit_coeff;
		/* 0x0074 */	float				threshold;
	}; // struct oxygen_tank::item_influence


			oxygen_tank::item_influence const*	find_influence				( pcstr body_part_name, pcstr hit_type );

private:
	/* 0x0000 */	/* inventory_item */
	/* 0x0118 */	bool							m_active;
	/* 0x011c */	scheduler::identifier			m_scheduler_identifier;
	/* 0x0120 */	u32								m_amount_ms;
	/* 0x0124 */	u32								m_max_amount;
	/* 0x0128 */	oxygen_tank::item_influence*	m_influences;
	/* 0x012c */	u8								m_influences_count;
}; // class oxygen_tank

STATIC_SIZE_ASSERT(oxygen_tank, 0x130);

} // namespace survarium

#endif // #ifndef OXYGEN_TANK_H_INCLUDED
