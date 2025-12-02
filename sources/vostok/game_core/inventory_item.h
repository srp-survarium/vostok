////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INVENTORY_ITEM_H_INCLUDED
#define INVENTORY_ITEM_H_INCLUDED

#include <vostok/game_core/interactive_object.h>

namespace survarium {

class inventory;			// sushi@TODO
class inventory_item_props;	// sushi@TODO
class game_world_object;	// sushi@TODO

// sushi@TODO: Move to a proper place
enum profile_slot_enum {
	helmet_slot        = 0x0000,
	mask_slot          = 0x0001,
	torso_slot         = 0x0002,
	back_slot          = 0x0003,
	pants_slot         = 0x0004,
	gloves_slot        = 0x0005,
	boots_slot         = 0x0006,
	weapon1_slot       = 0x0007,
	ammo1_weapon1_slot = 0x0008,
	ammo2_weapon1_slot = 0x0009,
	weapon2_slot       = 0x000a,
	ammo1_weapon2_slot = 0x000b,
	ammo2_weapon2_slot = 0x000c,
	quick_slot1        = 0x000d,
	quick_slot2        = 0x000e,
	quick_slot3        = 0x000f,
	quick_slot4        = 0x0010,
	quick_slot5        = 0x0011,
	quick_slot6        = 0x0012,
	max_slots_count    = 0x0013,
	invalid_slot       = 0x0013
};

class inventory_item : public interactive_object , public boost::noncopyable {
public:
	enum action_behaviour_type {
		disabled				= 0x0000,
		use_silent				= 0x0001,
		inventory_active_item	= 0x0002
	};

public:
												inventory_item					( inventory_item::action_behaviour_type type );

	virtual	void								set_inventory					( inventory* inv, profile_slot_enum slot );
			inventory&							get_inventory					( ) const;

			profile_slot_enum					profile_slot_id					( ) const;

	inline	inventory_item::action_behaviour_type const&	get_action_behaviuor( ) const { /* no source */ }

	virtual	void								action							( bool key_down );

	virtual	void								remove							( ) { /* no source */ }

	virtual	void								holder_assigned					( ) { /* no source */ }
	virtual	void								holder_removed					( ) { /* no source */ }

			void								set_amount						( u16 value );
			u16									amount							( ) const;

	inline	void								set_dict_id						( u16 arg_0 ) { /* no source */ }
	inline	u16									get_dict_id						( ) { /* no source */ }

	virtual	bool								get_item_props					( inventory_item_props& props );

	virtual	weapon_core const*					cast_weapon_core				( ) const override { /* no source */ }
	virtual	weapon_core*						cast_weapon_core				( ) override;

	virtual	void								serialize						( network_core::udp_match_packet& packet, u32 client_offset ) const override;
	virtual	void								deserialize						( network_core::packet_reader& reader ) override;

	virtual	void							serialize_game_world_object_header	( game_world_object& object, network_core::udp_match_packet& packet ) const;
	virtual	void								deserialize_game_world_object	( network_core::packet_reader& reader );

private:
	/* 0x0000 */	/* interactive_object */
	/* 0x0108 */	/* boost::noncopyable */
	/* 0x0108 */	inventory_item::action_behaviour_type	m_action_behaviuor;
	/* 0x010c */	inventory*								m_inventory;
	/* 0x0110 */	profile_slot_enum						m_slot_id;
	/* 0x0114 */	u16										m_amount;
	/* 0x0116 */	u16										m_dict_id;
}; // class inventory_item

STATIC_SIZE_ASSERT(inventory_item, 0x118);

} // namespace survarium

#endif // #ifndef INVENTORY_ITEM_H_INCLUDED
