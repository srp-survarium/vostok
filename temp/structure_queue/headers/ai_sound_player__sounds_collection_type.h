////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef AI_SOUND_PLAYER_SOUNDS_COLLECTION_TYPE_H_INCLUDED
#define AI_SOUND_PLAYER_SOUNDS_COLLECTION_TYPE_H_INCLUDED

/* INCLUDES */
class vostok::resources::child_resource_ptr<vostok::sound::sound_emitter,vostok::resources::unmanaged_intrusive_base>;
class vostok::ai::sound_collection_types;
class vostok::sound::sound_emitter;

/* FORWARD REFS */
class survarium::ai_sound_player;

namespace survarium {

enum vostok::ai::sound_collection_types
{
	sound_collection_type_undefined			= -0x1,
	sound_collection_type_item_picking		= 0x0,
	sound_collection_type_item_dropping		= 0x1,
	sound_collection_type_item_hiding		= 0x2,
	sound_collection_type_item_taking		= 0x3,
	sound_collection_type_item_using		= 0x4,
	sound_collection_type_weapon_shooting	= 0x5,
	sound_collection_type_weapon_reloading	= 0x6,
	sound_collection_type_weapon_empty_clicking	= 0x7,
	sound_collection_type_npc_dying_in_combat	= 0x8,
	sound_collection_type_npc_dying_in_water	= 0x9,
	sound_collection_type_npc_pain			= 0xa,
	sound_collection_type_npc_walking		= 0xb,
	sound_collection_type_npc_talking		= 0xc,
	sound_collection_type_npc_eating		= 0xd,
	sound_collection_type_world_ambient		= 0xe,
	sound_collection_type_count				= 0xf,
};

struct ai_sound_player::sounds_collection_type {
				sounds_collection_type	(
					ai_sound_player*				parent,
					ai::sound_collection_types		collection_type,
					sound::sound_emitter_ptr		emitter_ptr,
					u32								collection_priority
				);
	inline		~sounds_collection_type	( ) { /* no source */ }

public:
	/* 0x0000 */	ai::sound_collection_types		type;
	/* 0x0004 */	resources::child_resource_ptr< sound::sound_emitter, resources::unmanaged_intrusive_base >	emitter;
	/* 0x000c */	u32								priority;
}; // struct ai_sound_player::sounds_collection_type

STATIC_SIZE_ASSERT(ai_sound_player::sounds_collection_type, 0x10);

} // namespace survarium

#endif // #ifndef AI_SOUND_PLAYER_SOUNDS_COLLECTION_TYPE_H_INCLUDED
