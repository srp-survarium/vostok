// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ARTEFACT_LIFEBONE_CORE_H_INCLUDED
#define ARTEFACT_LIFEBONE_CORE_H_INCLUDED

#include <vostok/game_core/artefact_base.h>
#include <vostok/game_core/damage_protector.h>

namespace survarium {

class artefact_lifebone_core : public artefact_base , public damage_protector {
public:
	explicit									artefact_lifebone_core		( );
	virtual										~artefact_lifebone_core		( );

	virtual	void								action						( bool key_down ) override;
	virtual	void								holder_assigned				( ) override;
	virtual	void								holder_removed				( ) override;
	virtual	float								reduce_damage				(
													pcstr		body_part_name,
													pcstr		damage_type,
													const float		amount,
													const float		armor_piercing
												);
			void								load_core					( configs::binary_config_value config );

protected:
			void								switch_passive_mode_impl	( bool switch_on );
			void								activate_impl				( );
			bool								protect_affect				( pcstr __formal, hit_affects_type_enum affect );
private:
	virtual	void								activate					( base_player& user, engine& engine ) override { }
	virtual	void								deactivate					( ) override {}

	virtual	float4x4							transform					( ) const override { VOSTOK_UNREACHABLE_CODE( ); return float4x4( ).identity( ); }

	virtual	void								tick						( ) override { /* no source */ }

	virtual	bool								is_ready_to_be_deactivated	( ) const override { return true; /* no source */ }

	virtual	animation::mixing::expression		selected_animations			( mutable_buffer& buffer, const bool is_third_view ) const override { VOSTOK_UNREFERENCED_PARAMETERS( buffer, is_third_view ); VOSTOK_UNREACHABLE_CODE( ); return animation::mixing::expression( ); }

	virtual	void								on_player_model_added		( ) override { /* no source */ }
	virtual	void								on_player_model_removed		( ) override { /* no source */ }

	// claude@NOTE: rva 0xbc810, ICF-folded across oxygen_tank+medkit. animation_player has
	// no accessible copy ctor, so it is the one argument the target passes by address;
	// user_skeleton is a 4-byte resource_ptr copied bitwise through the ellipsis and the
	// two float4x4 go by value (0x40 each) - 0x94 pushed in total.
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

	virtual	void								serialize					( network_core::udp_match_packet& arg_0 ) const { /* no source */ }
	virtual	void								deserialize					( network_core::packet_reader& arg_0 ) override { /* no source */ }

	virtual	bool								is_sprinting				( ) const override { return true; /* no source */ }
public:
	enum { protects_count = 4 };

private:
	/* 0x0000 */	/* artefact_base */
	/* 0x0120 */	/* damage_protector */
	/* 0x0170 */	damage_protector		m_damage_protectors[protects_count];
	/* 0x02b0 */	bool					m_unlimited;
	/* 0x02b1 */	bool					m_passive_mode;
	/* 0x02b4 */	u32						m_cooldown_ms;
	/* 0x02b8 */	u32						m_last_used_time_ms;
}; // class artefact_lifebone_core

STATIC_SIZE_ASSERT(artefact_lifebone_core, 0x2C0);

} // namespace survarium

#endif // #ifndef ARTEFACT_LIFEBONE_CORE_H_INCLUDED
