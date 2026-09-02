// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef BOOBY_TRAP_SET_CORE_H_INCLUDED
#define BOOBY_TRAP_SET_CORE_H_INCLUDED

#include <vostok/game_core/inventory_item.h>

namespace survarium {

class booby_trap_core;
class booby_trap_set_core_cook;
typedef resources::resource_ptr< booby_trap_core, resources::unmanaged_intrusive_base > booby_trap_core_ptr;

class game_material_manager;

class booby_trap_set_core : public inventory_item {
public:
	struct config_params {
		inline	explicit	config_params	( ) :
			max_slope_cos			( 0.0f ),
			max_distance			( 0.0f ),
			armed_life_time			( 0 ),
			fired_life_time			( 0 ),
			disarmed_life_time		( 0 ),
			defuse_time				( 0 ),
			defuse_by_hit			( false ),
			material_can_place_test	( false ),
			material_can_stick_test	( false )
		{ }

		/* 0x0000 */	float		max_slope_cos;
		/* 0x0004 */	float		max_distance;
		/* 0x0008 */	u32			armed_life_time;
		/* 0x000c */	u32			fired_life_time;
		/* 0x0010 */	u32			disarmed_life_time;
		/* 0x0014 */	u32			defuse_time;
		/* 0x0018 */	bool		defuse_by_hit;
		/* 0x0019 */	bool		material_can_place_test;
		/* 0x001a */	bool		material_can_stick_test;
	}; // struct booby_trap_set_core::config_params

	struct apply_damage {
		/* 0x0000 */	char		body_part[16];
		/* 0x0010 */	char		hit_type[16];
		/* 0x0020 */	float		amount;
		/* 0x0024 */	float		armor_piercing;
	}; // struct apply_damage

	typedef buffer_vector< apply_damage > damage_parameters_type;

	virtual	void								insert_trap						( booby_trap_core& trap, float4x4 const& transform );
	virtual	void								remove_trap						( booby_trap_core& trap );

	virtual	void								on_trap_fired					( booby_trap_core& trap ) { /* no source */ }
	virtual	void								on_trap_disarmed				( booby_trap_core& trap ) { /* no source */ }

	inline damage_parameters_type const& damage_parameters				( ) const { return m_damage_parameters; }

	// claude@NOTE: config()/traps() carry the correct one-line bodies and pair in this TU,
	// but the target compiled them /Ox (frameless, this in eax/ecx, no ebp frame) while
	// our base is /Od; the residual is the optimization-level wall, not source-steerable.
			booby_trap_set_core::config_params const&
												config							( ) const { return m_config; }

private:
	// private in the target: mangled @@EAE (private virtual), not @@UAE - pairing depends on it
	virtual	void								deserialize_game_world_object	( network_core::packet_reader& reader ) override;

protected:
	virtual	void								remove							( ) override;

public:
	virtual	void								serialize_game_world_object_header(
													booby_trap_core const&				trap,
													network_core::udp_match_packet&		packet
												) const;

private:
	// claude@NOTE: activate/deactivate/transform/selected_animations are empty/unreachable
	// virtuals that ICF-fold to shared targets (no distinct symbol in either index), so they
	// are unpairable as standalones; the idiomatic bodies below are the faithful shapes.
	virtual	void								activate						( base_player& user, engine& engine ) override { VOSTOK_UNREACHABLE_CODE( ); }
	virtual	void								deactivate						( ) override { VOSTOK_UNREACHABLE_CODE( ); }
	virtual	float4x4							transform						( ) const override { VOSTOK_UNREACHABLE_CODE( ); }

	virtual	void								tick							( ) override { /* no source */ }
	virtual	bool								is_ready_to_be_deactivated		( ) const override { return true; /* sushi@TODO no source */ }

	virtual	animation::mixing::expression		selected_animations				( mutable_buffer& buffer, const bool is_third_view ) const override
	{
		VOSTOK_UNREFERENCED_PARAMETERS( buffer, is_third_view );
		VOSTOK_UNREACHABLE_CODE( );
	}

	virtual	void								on_player_model_added			( ) override { /* no source */ }
	virtual	void								on_player_model_removed			( ) override { /* no source */ }

	virtual	void								update_bones_matrices			(
													animation::skeleton_ptr const&		user_skeleton,
													float4x4* const						user_matrices,
													const u32									user_matrices_count,
													const u32									current_time_in_ms,
													float4x4&							character_head_transform,
													float4x4&							character_transform,
													animation::animation_player const&	animation_player
												) override;

	virtual	bool								is_sprinting					( ) const override { return true; /* sushi@TODO no source */ }


protected:
												booby_trap_set_core				( );
	virtual										~booby_trap_set_core			( );


			void								load							( configs::binary_config_value const& config );

	typedef buffer_vector< booby_trap_core_ptr > booby_traps_type;

	virtual	game_material_manager const&		get_game_material_manager		( ) = 0;
			bool								get_visible_place_transform		( float4x4& result );

			booby_trap_core_ptr*				try_place_trap					( );
	inline	void								remove_traps					( ) { /* no source */ }

			booby_traps_type&						traps					( )			{ return m_traps; }
	inline	booby_traps_type const&					traps					( ) const	{ return m_traps; }

			u8									trap_index						( booby_trap_core const& trap ) const;

	typedef inventory_item super;

private:
	inline	u8									count_active_traps				( ) const { /* no source */ }

	inline	void								append_inactive_trap_index_to_packet(
													booby_trap_core_ptr const&			arg_0,
													network_core::udp_match_packet&		arg_1
												) const { /* no source */ }

private:
			void								remove_trap_impl				( booby_trap_core& trap );
			void								remove_trap_if_active			( booby_trap_core_ptr& trap );

private:
	/* 0x0000 */	/* inventory_item */
	/* 0x0118 */	booby_traps_type						m_traps;
	/* 0x0120 */	damage_parameters_type				m_damage_parameters;
	/* 0x0128 */	booby_trap_set_core::config_params		m_config;
	/* 0x0144 */	booby_trap_core_ptr*					m_traps_buffer;
private:
	friend class booby_trap_set_core_cook;
}; // class booby_trap_set_core

STATIC_SIZE_ASSERT(booby_trap_set_core, 0x148);

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_SET_CORE_H_INCLUDED
