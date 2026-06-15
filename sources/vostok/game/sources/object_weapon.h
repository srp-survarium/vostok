////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_WEAPON_H_INCLUDED
#define OBJECT_WEAPON_H_INCLUDED

// the canonical dump re-prints ai::weapon_types_enum here; it lives in vostok/ai
#include <vostok/ai/weapon.h>
#include <vostok/ai/game_object.h>

namespace survarium {

class human_npc;

// pulled ahead of batch 7 (human_npc::npc_game_attributes' intrusive-list
// member pointer &object_weapon::m_next needs the complete type); out-of-line
// bodies in the object_weapon.cpp compiland

// void* object_weapon::`scalar deleting destructor'( u32 ) // FUNCTION BODY[0xabef0]: <0xabee0>|0x000|      :'43'	{

class object_weapon : public ai::weapon , public ai::game_object {
	// human_npc::npc_game_attributes names &object_weapon::m_next (private)
	friend class human_npc;
public:
										object_weapon			(
											ai::weapon_types_enum		type,
											pcstr						name,
											u32							id,
											u32							ammo_count
										);
	virtual								~object_weapon			( );

	// STATE[STUB]
	virtual	ai::game_object const*		cast_game_object		( ) const override
	{
		return this;

		// FUNCTION BODY[0xabeb0]
		// <0xabeb0>|0x000|      :'31'	{
		// ******
	}

	// STATE[STUB]
	virtual	ai::weapon_types_enum		get_type				( ) const override
	{
		return m_type;

		// FUNCTION BODY[0x3c580]
		// <0x3c580>|0x000|      :'32'	{
		// ******
	}

	// STATE[STUB]
	virtual	bool						is_loaded				( ) const override
	{
		return false;

		// FUNCTION BODY[0xabec0]
		// <0xabec0>|0x000|      :'33'	{
		// ******
	}

	virtual	pcstr						get_name				( ) const override { /* no source */ return m_name; }

	// STATE[STUB]
	virtual	u32							get_id					( ) const override
	{
		return m_id;

		// FUNCTION BODY[0xabed0]
		// <0xabed0>|0x000|      :'36'	{
		// ******
	}

	// STATE[STUB]
	virtual	ai::npc*					cast_npc				( ) override
	{
		return NULL;

		// FUNCTION BODY[0x427c0]
		// <0x427c0>|0x000|      :'38'	{
		// ******
	}

	virtual	ai::npc const*				cast_npc				( ) const override { /* no source */ return NULL; }

	// STATE[STUB]
	virtual	ai::weapon*					cast_weapon				( ) override
	{
		return this;

		// FUNCTION BODY[0xabb00]
		// <0xabb00>|0x000|      :'40'	{
		// ******
	}

	virtual	ai::weapon const*			cast_weapon				( ) const override { /* no source */ return this; }

	// STATE[STUB]
	virtual	float						get_velocity			( ) const override
	{
		return 0.0f;

		// FUNCTION BODY[0xab8f0]
		// <0xab8f0>|0x000|      :'42'	{
		// ******
	}

	virtual	float						get_luminosity			( ) const override { /* no source */ return 0.0f; }
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
