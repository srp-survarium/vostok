// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef WEAPON_AMMUNITION_H_INCLUDED
#define WEAPON_AMMUNITION_H_INCLUDED

#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/ammo_type_enum.h>

#include <vostok/game_core/engine.h>
#include <vostok/game_core/base_player.h>

namespace survarium {

struct inventory_item_instance;
struct base_player;
struct engine;

class weapon_ammunition : public inventory_item {
public:
	typedef inventory_item super;

	explicit			weapon_ammunition			( );

	inline	float		distance					( ) const { return m_distance; }
	inline	float		dispersion					( ) const { return m_dispersion; }
	inline	float		ricochet_angle				( ) const { return m_ricochet_angle; }
	inline	float		damage						( ) const { return m_damage; }
	inline	float		impulse						( ) const { return m_impulse; }
	inline	float		pierce						( ) const { return m_pierce; }
	inline	float		air_resistance				( ) const { return m_air_resistance; }
	inline	float		muzzle_speed				( ) const { return m_muzzle_speed; }
	inline	u16			buck_shot					( ) const { return m_buck_shot; }
	inline	u16			game_material_id			( ) const { return m_game_material_id; }
	inline	bool		tracer						( ) const { return m_tracer; }

			void		load						( configs::binary_config_value const& cfg );

private:
	// claude@NOTE: activate/deactivate/transform/selected_animations are empty/unreachable
	// virtuals that ICF-fold (no distinct symbol in either index) - unpairable standalones;
	// the idiomatic bodies below are the faithful shapes.
	// claude@MATCH: the target's eater call is UNGUARDED (no identity(false) round-trip) and
	// pushes the two reference POINTERS, so the source called the helper directly rather than
	// through VOSTOK_UNREFERENCED_PARAMETERS - the macro's never-taken block would copy
	// base_player by value (0x11C rep movsd). See patterns/unreferenced-params-eater.md.
	virtual	void		activate					( base_player& user, engine& engine ) override
	{
		ASSERT( UNKNOWN_EXPRESSION );
		vostok::detail::unreferenced_parameter_helper( & user, & engine );
	}
	virtual	void		deactivate					( ) override { }
	virtual	float4x4	transform					( ) const override { VOSTOK_UNREACHABLE_CODE( ); }

	virtual	void		tick						( ) override { /* no source */ }

	virtual	bool		is_ready_to_be_deactivated	( ) const override { return true; /* sushi@TODO no source */ }

	virtual	animation::mixing::expression
						selected_animations			( mutable_buffer& buffer, const bool is_third_view ) const override { VOSTOK_UNREFERENCED_PARAMETERS( buffer, is_third_view ); VOSTOK_UNREACHABLE_CODE( ); }

	virtual	void		on_player_model_added		( ) override { /* no source */ }
	virtual	void		on_player_model_removed		( ) override { /* no source */ }

private:
	virtual	void		update_bones_matrices		(
							animation::skeleton_ptr const&		user_skeleton,
							float4x4* const						user_matrices,
							const u32									user_matrices_count,
							const u32									current_time_in_ms,
							float4x4&							character_head_transform,
							float4x4&							character_transform,
							animation::animation_player const&	animation_player
						) override;

	virtual	void		serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const override { inventory_item::serialize( packet, client_offset ); }
	virtual	void		deserialize					( network_core::packet_reader& reader ) override { inventory_item::deserialize( reader ); }

	virtual	bool		is_sprinting				( ) const override { return false; /* sushi@TODO no source */ }

private:
	/* 0x0000 */	/* inventory_item */
	/* 0x0118 */	float						m_distance;
	/* 0x011c */	float						m_dispersion;
	/* 0x0120 */	float						m_damage;
	/* 0x0124 */	float						m_impulse;
	/* 0x0128 */	float						m_pierce;
	/* 0x012c */	float						m_air_resistance;
	/* 0x0130 */	float						m_ricochet_angle;
	/* 0x0134 */	float						m_muzzle_speed;
	/* 0x0138 */	s16							m_buck_shot;
	/* 0x013a */	s16							m_game_material_id;
	/* 0x013c */	bool						m_tracer;
	/* 0x0140 */	ammo_type_enum				m_ammo_type;
	/* 0x0144 */	inventory_item_instance*	m_source;
}; // class weapon_ammunition

STATIC_SIZE_ASSERT(weapon_ammunition, 0x148);

typedef resources::resource_ptr< weapon_ammunition, resources::unmanaged_intrusive_base > weapon_ammunition_ptr;

} // namespace survarium

#endif // #ifndef WEAPON_AMMUNITION_H_INCLUDED
