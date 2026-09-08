// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OXYGEN_TANK_H_INCLUDED
#define OXYGEN_TANK_H_INCLUDED

#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/damage_protector.h>

namespace survarium {

class items_cook;

class oxygen_tank : public inventory_item {
public:
	typedef inventory_item super;

	virtual	void								action						( bool key_down ) override;
	virtual	bool								get_item_props				( inventory_item_props& props ) override;

private:
												oxygen_tank					( );
	virtual										~oxygen_tank				( );

			void								load						( configs::binary_config_value config );

	protected:
	inline	bool								empty						( ) const { return !m_amount_ms; }

			void								set_active					( bool bactive );
			void								active_tick					( const u32 frame_time_ms );
			float								reduce_damage				(
													pcstr		body_part_name,
													pcstr		damage_type,
													const float		amount,
													const float		armor_piercing
												);

private:
	// claude@NOTE: activate/deactivate/transform/selected_animations are empty/unreachable
	// virtuals that ICF-fold (no distinct symbol in either index) - unpairable standalones,
	// faithful idioms below. update_bones_matrices ICF-folds with the medkit and
	// artefact_lifebone_core copies at rva 0xbc810.
	virtual	void								activate					( base_player& user, engine& engine ) override { }
	virtual	void								deactivate					( ) override { }
	virtual	float4x4							transform					( ) const override { VOSTOK_UNREACHABLE_CODE( ); return float4x4( ).identity( ); }

	virtual	void								tick						( ) override { /* no source */ }

	virtual	bool								is_ready_to_be_deactivated	( ) const override { return false; /* no source */ }
	virtual	animation::mixing::expression		selected_animations			( mutable_buffer& buffer, const bool is_third_view ) const override { VOSTOK_UNREFERENCED_PARAMETERS( buffer, is_third_view ); VOSTOK_UNREACHABLE_CODE( ); return animation::mixing::expression( ); }

	virtual	void								on_player_model_added		( ) override { /* no source */ }
	virtual	void								on_player_model_removed		( ) override { /* no source */ }
	virtual	void								update_bones_matrices		(
													animation::skeleton_ptr const&		user_skeleton,
												float4x4* const						user_matrices,
												const u32							user_matrices_count,
												const u32							current_time_in_ms,
													float4x4&							character_head_transform,
													float4x4&							character_transform,
													animation::animation_player const&	animation_player
												) override {
													VOSTOK_UNREFERENCED_PARAMETERS(
														user_skeleton,
														user_matrices,
														user_matrices_count,
														current_time_in_ms,
														character_head_transform,
														character_transform,
														& animation_player
													);
												}
	virtual	void								serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const override { inventory_item::serialize( packet, client_offset ); }
	virtual	void								deserialize					( network_core::packet_reader& reader ) override { inventory_item::deserialize( reader ); }

	virtual	bool								is_sprinting				( ) const override { return false; /* no source */ }

protected:
	/* 0x0000 */	/* inventory_item */
	/* 0x0118 */	bool					m_active;
	/* 0x011c */	scheduler::identifier	m_scheduler_identifier;
	/* 0x0120 */	u32						m_amount_ms;
	/* 0x0124 */	u32						m_max_amount;

	struct item_influence {
		/* 0x0000 */	damage_protector	protector;
		/* 0x0050 */	char				body_part_name[16];
		/* 0x0060 */	char				hit_type[16];
		/* 0x0070 */	float				hit_coeff;
		/* 0x0074 */	float				threshold;
	}; // struct oxygen_tank::item_influence

	/* 0x0128 */	item_influence*			m_influences;
	/* 0x012c */	u8						m_influences_count;

			oxygen_tank::item_influence const*	find_influence				( pcstr body_part_name, pcstr hit_type );

private:
	friend class items_cook;
}; // class oxygen_tank

STATIC_SIZE_ASSERT(oxygen_tank, 0x130);

} // namespace survarium

#endif // #ifndef OXYGEN_TANK_H_INCLUDED
