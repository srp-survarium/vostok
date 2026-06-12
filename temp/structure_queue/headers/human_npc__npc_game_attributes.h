////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef HUMAN_NPC_NPC_GAME_ATTRIBUTES_H_INCLUDED
#define HUMAN_NPC_NPC_GAME_ATTRIBUTES_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<32>;
class vostok::math::color;
typedef vostok::intrusive_list<survarium::object_weapon,survarium::object_weapon *,12,vostok::threading::mutex,vostok::size_policy,vostok::no_debug_policy>
	survarium::object_weapon_list;
class survarium::object_weapon;

namespace survarium {

struct human_npc::npc_game_attributes {
												npc_game_attributes	( );

			human_npc::npc_game_attributes&		operator=			( human_npc::npc_game_attributes& other );

	inline										~npc_game_attributes( ) { /* no source */ }

public:
	/* 0x0000 */	object_weapon_list		weapons;
	/* 0x0030 */	float3					initial_position;
	/* 0x003c */	float3					initial_scale;
	/* 0x0048 */	float3					initial_rotation;
	/* 0x0054 */	math::color				debug_draw_color;
	/* 0x0058 */	fixed_string< 32 >		name;
	/* 0x0084 */	fixed_string< 32 >		description;
	/* 0x00b0 */	float					initial_velocity;
	/* 0x00b4 */	float					initial_luminosity;
	/* 0x00b8 */	u32						id;
	/* 0x00bc */	u32						group_id;
	/* 0x00c0 */	u32						class_id;
	/* 0x00c4 */	u32						outfit_id;
}; // struct human_npc::npc_game_attributes

STATIC_SIZE_ASSERT(human_npc::npc_game_attributes, 0xC8);

} // namespace survarium

#endif // #ifndef HUMAN_NPC_NPC_GAME_ATTRIBUTES_H_INCLUDED
