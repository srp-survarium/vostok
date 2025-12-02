////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INVENTORY_ITEM_H_INCLUDED
#define INVENTORY_ITEM_H_INCLUDED

#include <vostok/game_core/interactive_object.h>
#include <vostok/game_core/profile_slot_enum.h>

namespace survarium {

class inventory;
struct inventory_item_props;
class game_world_object;	// sushi@TODO

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

typedef resources::resource_ptr< inventory_item, resources::unmanaged_intrusive_base > inventory_item_ptr;

} // namespace survarium

#endif // #ifndef INVENTORY_ITEM_H_INCLUDED
