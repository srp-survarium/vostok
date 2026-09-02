// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef ARTEFACT_SPORES_CORE_H_INCLUDED
#define ARTEFACT_SPORES_CORE_H_INCLUDED

#include <vostok/game_core/artefact_base.h>
#include <vostok/game_core/missile_weapon_core.h>

namespace vostok {
namespace physics {
	class bt_ghost_object;
}
}

namespace survarium {

struct hit_initiator;


class artefact_spores_core : public missile_weapon_core , public artefact_base {
public:
	inline explicit	artefact_spores_core	( physics::world* world, physics::bt_static_rigid_body* rigid_body ) :
		missile_weapon_core( world, rigid_body )
	{
	}
	virtual			~artefact_spores_core	( ) { /* no source */ }

	inline	void	load					( configs::binary_config_value const& arg_0 ) { /* no source */ }
	inline	void	set_owner				( hit_initiator* arg_0 ) { /* no source */ }

	virtual	void	tick					( u32 arg_0, float4x4 const& arg_1 ) override { /* no source */ }
	virtual	void	activate				( u32 arg_0, float4x4 const& arg_1 ) override { /* no source */ }

protected:
	virtual	void	on_contact_callback		( physics::base_physics_object* arg_0, physics::base_physics_object* arg_1, float3 const& arg_2 ) override { /* no source */ }

	virtual	void	cloud_destroyed			( ) { /* no source */ }

private:
	inline	void	infect_objects_in_cloud	( ) { /* no source */ }

private:
	/* 0x0000 */	/* missile_weapon_core */
	/* 0x0040 */	/* artefact_base */
	/* 0x0158 */	physics::bt_ghost_object*		m_ghost_object;
	/* 0x015c */	hit_initiator*					m_owner;
	/* 0x0160 */	pcstr							m_body_part_name;
	/* 0x0164 */	pcstr							m_hit_type;
	/* 0x0168 */	float							m_hit_amount;
	/* 0x016c */	float							m_armor_piercing;
	/* 0x0170 */	u32								m_cloud_duration;
	/* 0x0174 */	u32								m_explosion_time;
	/* 0x0178 */	u32								m_last_tick_time;
}; // class artefact_spores_core

STATIC_SIZE_ASSERT(artefact_spores_core, 0x180);

} // namespace survarium

#endif // #ifndef ARTEFACT_SPORES_CORE_H_INCLUDED
