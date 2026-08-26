////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BOOBY_TRAP_CORE_H_INCLUDED
#define BOOBY_TRAP_CORE_H_INCLUDED

#include <vostok/game_core/game_world_object.h>
#include <vostok/game_core/hittable_object.h>
#include <vostok/game_core/collision_sensor.h>
#include <vostok/game_core/usable_object.h>

#include <vostok/game_core/booby_trap_set_core.h>
#include <vostok/game_core/scheduler.h>

namespace vostok {
namespace physics {
	struct world;
	class bt_static_rigid_body;
}
namespace collision{
	class bone_collision_data;
}
}

namespace survarium {

class booby_trap_core_cook;

enum booby_trap_state {
	booby_trap_state_removed	= 0x0000,
	booby_trap_state_armed		= 0x0001,
	booby_trap_state_fired		= 0x0002,
	booby_trap_state_disarmed	= 0x0003,
	booby_trap_states_count		= 0x0004,
};


class booby_trap_core : public game_world_object , public hittable_object , public collision_sensor , public usable_object {
protected:
	explicit								booby_trap_core				( );
	virtual									~booby_trap_core			( );

	virtual	void							load						( configs::binary_config_value const& config ) override;
	inline	void							load_collision				( configs::binary_config_value const& config ) { /* no source */ }
			void							load_aabb					( configs::binary_config_value const& config );

public:
	virtual	void							set_transform				( float4x4 const& transform );

	virtual	void							insert						( physics::world* world, float4x4 const& transform, scheduler& scheduler );
	virtual	void							remove						( scheduler& scheduler );

	inline	float4x4 const&					transform					( ) const { return m_transform; }
	inline	void							set_owner					( booby_trap_set_core* owner ) { m_owner = owner; }
	inline	bool							is_active					( ) const { return m_trap_state != booby_trap_state_removed; }

private:
	virtual	booby_trap_set_core const*		owner						( ) const override;
	virtual	booby_trap_set_core*			owner						( ) override;

	virtual	void							serialize					( network_core::udp_match_packet& packet ) const override;
	virtual	void							deserialize					( network_core::packet_reader& reader ) override;

	virtual	void							hit							(
												hit_initiator const* const	initiator,
												u32	const					bone_index,
												pcstr						damage_type,
												float const					amount,
												float const					armor_piercing,
												bullet*	const				bullet
											) override;
	virtual	void							hit							(
												hit_initiator const* const				initiator,
												collision::bone_collision_data const&	bone_data,
												pcstr									damage_type,
												float const								amount,
												float const								armor_piercing,
												bullet*	const							bullet
											) override;

	// claude@MATCH: target is FRAMELESS for this `this`-unused leaf (`fldz;ret`); /Od always frames -> unpairable. fldz correct. Private virtual `EBE`.
	virtual	float							get_speed					( ) const override { return 0.0f; }
	virtual	void							on_enter					( buffer_vector<physics::base_physics_object *> const& objects ) override;
	virtual	void							tick						( const u32 time_delta_ms, const u32 current_time_ms ) override;

protected:
	virtual	bool							use_initialize				( usable_object_user_data* user ) override;
	virtual	bool							use_execute					( usable_object_user_data* user ) override;
	virtual	bool							use_finalize				( usable_object_user_data* user ) override;
	virtual	pcstr							use_info					( usable_object_user_data* user ) override;

	/* 0x0168 */	scheduler::identifier		m_scheduler_identifier;
	/* 0x016c */	booby_trap_set_core*		m_owner;
	/* 0x0170 */	booby_trap_state			m_trap_state;

	virtual	void							defuse_completed			( );
	virtual	void							switch_to_state				( booby_trap_state new_state );

	virtual	void							register_tick				( scheduler& scheduler );
	virtual	void							unregister_tick				( scheduler& scheduler );

private:
			void							apply_damage				( hit_initiator const* const initiator, hit_receiver* const receiver );
			bool							can_defuse					( base_player const* user ) const;
			void							on_state_timer_finished		( );

protected:
	/* 0x0000 */	/* game_world_object */
	/* 0x0110 */	/* hittable_object */
	/* 0x0124 */	/* collision_sensor */
	/* 0x0148 */	/* usable_object */
private:
	/* 0x0174 */	float4x4					m_transform;
	/* 0x01b4 */	u32							m_state_timer;
private:
	// the game-module runtime booby_trap reads inherited state (m_trap_state /
	// m_transform / m_owner) directly in switch_to_state / on_new_state; friendship
	// is codegen-neutral (emits no bytes, not recorded in the PDB).
	friend class booby_trap;
	friend class booby_trap_core_cook;
}; // class booby_trap_core

STATIC_SIZE_ASSERT(booby_trap_core, 0x1B8);

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_CORE_H_INCLUDED
