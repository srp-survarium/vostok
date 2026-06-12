////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_WEAPON_H_INCLUDED
#define OBJECT_WEAPON_H_INCLUDED

/* INCLUDES */
class survarium::object_weapon;
struct vostok::ai::game_object;
struct vostok::ai::weapon;
enum vostok::ai::weapon_types_enum;

/* FORWARD REFS */
class vostok::ai::npc;

namespace survarium {

enum vostok::ai::weapon_types_enum
{
	weapon_type_melee		= 0x0,
	weapon_type_sniper		= 0x1,
	weapon_type_heavy		= 0x2,
	weapon_type_energy		= 0x3,
	weapon_type_light		= 0x4,
	weapon_types_count		= 0x5,
};

class object_weapon : public ai::weapon , public ai::game_object {
public:
										object_weapon			(
											ai::weapon_types_enum		type,
											pcstr						name,
											u32							id,
											u32							ammo_count
										);
	virtual								~object_weapon			( );

	virtual	ai::game_object const*		cast_game_object		( ) const override { /* no source */ }

	virtual	ai::weapon_types_enum		get_type				( ) const override { /* no source */ }

	virtual	bool						is_loaded				( ) const override { /* no source */ }

	virtual	pcstr						get_name				( ) const override { /* no source */ }
	virtual	u32							get_id					( ) const override { /* no source */ }

	virtual	ai::npc*					cast_npc				( ) override { /* no source */ }
	virtual	ai::npc const*				cast_npc				( ) const override { /* no source */ }
	virtual	ai::weapon*					cast_weapon				( ) override { /* no source */ }
	virtual	ai::weapon const*			cast_weapon				( ) const override { /* no source */ }

	virtual	float						get_velocity			( ) const override { /* no source */ }
	virtual	float						get_luminosity			( ) const override { /* no source */ }
	virtual	float3						get_random_surface_point( const u32 current_time ) const override;

	virtual	float4x4					local_to_cell			( float3 const& requester ) const override;

private:
	/* 0x0000 */	/* ai::weapon */
	/* 0x0004 */	/* ai::game_object */
	/* 0x000c */	object_weapon*				m_next;
	/* 0x0010 */	ai::weapon_types_enum		m_type;
	/* 0x0014 */	pcstr						m_name;
	/* 0x0018 */	u32							m_id;
	/* 0x001c */	u32							m_ammo_count;
}; // class object_weapon

STATIC_SIZE_ASSERT(object_weapon, 0x20);

} // namespace survarium

#endif // #ifndef OBJECT_WEAPON_H_INCLUDED
