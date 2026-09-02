// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef MISSILE_WEAPON_CORE_H_INCLUDED
#define MISSILE_WEAPON_CORE_H_INCLUDED

namespace vostok {
namespace physics {
	class bt_static_rigid_body;
	class base_physics_object;
	struct world;
}
}

namespace survarium {

class missile_weapon_core {
public:
	enum activation_type {
		immediate	= 0x0000,
		delayed		= 0x0001,
	};

	inline	explicit		missile_weapon_core		( physics::world* physics_world, physics::bt_static_rigid_body* rigid_body ) : m_physics_world( physics_world ), m_rigid_body( rigid_body ) { }
	virtual					~missile_weapon_core	( ) { /* no source */ }

	inline	void			load					( configs::binary_config_value const& arg_0 ) { /* no source */ }

	virtual	void			tick					( u32 arg_0, float4x4 const& arg_1 ) { /* no source */ }
	virtual	void			activate				( u32 arg_0, float4x4 const& arg_1 ) { /* no source */ }
	virtual	void			throw_weapon			( u32 arg_0, float4x4 const& arg_1, float3 const& arg_2 ) { /* no source */ }

	virtual	void			on_contact_callback		( physics::base_physics_object* arg_0, physics::base_physics_object* arg_1, float3 const& arg_2 ) { /* no source */ }

	inline	activation_type	get_activation_type		( ) const { return m_type; }

public:
	inline	void			get_transform			( float4x4& arg_0 ) const { /* no source */ }

protected:
	inline	void			remove_physics_object	( ) { /* no source */ }
	inline	physics::world*	physics_world			( ) { return m_physics_world; }

private:
	/* 0x0008 */	physics::world*					m_physics_world;
	/* 0x000c */	physics::bt_static_rigid_body*	m_rigid_body;
	/* 0x0010 */	boost::function< void( physics::base_physics_object*, physics::base_physics_object*, float3 const& ) >
										m_collide_callback;
	/* 0x0030 */	float							m_impulse;
	/* 0x0034 */	float							m_dispersion;
	/* 0x0038 */	activation_type					m_type;
}; // class missile_weapon_core

STATIC_SIZE_ASSERT(missile_weapon_core, 0x40);

} // namespace survarium

#endif // #ifndef MISSILE_WEAPON_CORE_H_INCLUDED
