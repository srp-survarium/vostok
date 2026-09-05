// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OBJECT_WEAPON_H_INCLUDED
#define OBJECT_WEAPON_H_INCLUDED

#include <vostok/ai/weapon.h>
#include <vostok/ai/game_object.h>

namespace survarium {

class object_weapon : public ai::weapon , public ai::game_object {
public:
										object_weapon			(
											ai::weapon_types_enum		type,
											pcstr						name,
											u32							id,
											u32							ammo_count = 32
										);
	virtual								~object_weapon			( );

	virtual	ai::game_object const*		cast_game_object		( ) const override { return this; }

	virtual	ai::weapon_types_enum		get_type				( ) const override { return m_type; }

	virtual	bool						is_loaded				( ) const override { return m_ammo_count > 0; }

	virtual	pcstr						get_name				( ) const override { return m_name; }

	virtual	u32							get_id					( ) const override { return m_id; }

	virtual	ai::npc*					cast_npc				( ) override { return NULL; }

	virtual	ai::npc const*				cast_npc				( ) const override { return NULL; }

	virtual	ai::weapon*					cast_weapon				( ) override { return this; }

	virtual	ai::weapon const*			cast_weapon				( ) const override { return this; }

	virtual	float						get_velocity			( ) const override { return 0.0f; }

	virtual	float						get_luminosity			( ) const override { return 0.003f; }

	virtual	float3						get_random_surface_point( const u32 current_time ) const override;

	virtual	float4x4					local_to_cell			( float3 const& requester ) const override;

private:
	/* 0x0000 */	/* ai::weapon */
	/* 0x0004 */	/* ai::game_object */
public:
	/* 0x000c */	object_weapon*				m_next;
private:
	/* 0x0010 */	ai::weapon_types_enum		m_type;
	/* 0x0014 */	pcstr						m_name;
	/* 0x0018 */	u32							m_id;
	/* 0x001c */	u32							m_ammo_count;
}; // class object_weapon

STATIC_SIZE_ASSERT(object_weapon, 0x20);

} // namespace survarium

#endif // #ifndef OBJECT_WEAPON_H_INCLUDED
