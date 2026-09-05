// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INVENTORY_ITEM_H_INCLUDED
#define INVENTORY_ITEM_H_INCLUDED

#include <vostok/game_core/interactive_object.h>
#include <vostok/game_core/profile_slot_enum.h>
#include <vostok/game_core/game_world_object.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

class inventory;
struct inventory_item_props;
class game_world_object;

class inventory_item : public interactive_object , private boost::noncopyable {
public:
	enum action_behaviour_type {
		disabled				= 0x0000,
		use_silent				= 0x0001,
		inventory_active_item	= 0x0002
	};

public:
									explicit	inventory_item					( inventory_item::action_behaviour_type type );
	virtual	void								set_inventory					( inventory* inv, profile_slot_enum slot ) { m_inventory = inv; m_slot_id = slot; }
	inline	inventory&							get_inventory					( ) const { return *m_inventory; }
	inline	profile_slot_enum					profile_slot_id					( ) const { return m_slot_id; }
	inline	inventory_item::action_behaviour_type const&
											get_action_behaviuor			( ) const { return m_action_behaviuor; }
	virtual	void								action							( bool key_down ) { VOSTOK_UNREFERENCED_PARAMETER( key_down ); }
	virtual	void								remove							( ) { }

	virtual	void								holder_assigned					( ) { }
	virtual	void								holder_removed					( ) { }

	inline	void								set_amount						( const u16 value ) { m_amount = value; }

	inline	u16									amount							( ) const { return m_amount; }

	inline	void								set_dict_id						( u16 item_dict_id ) { m_dict_id = item_dict_id; }
	inline	u16									get_dict_id						( ) { return m_dict_id; }

	virtual	bool								get_item_props					( inventory_item_props& props );

	virtual	weapon_core*						cast_weapon_core				( ) override { return NULL; }
	virtual	weapon_core const*					cast_weapon_core				( ) const override { return NULL; }

	virtual	void								serialize						( network_core::udp_match_packet& packet, u32 client_offset ) const override;
	virtual	void								deserialize						( network_core::packet_reader& reader ) override;

	// sushi@TODO: verify the variadic-copy/unreachable source form; retail has no returning epilogue.
	virtual	void							serialize_game_world_object_header	( game_world_object& object, network_core::udp_match_packet& packet ) const { vostok::detail::unreferenced_parameter_helper( object, packet ); VOSTOK_UNREACHABLE_CODE( ); }
	virtual	void								deserialize_game_world_object	( network_core::packet_reader& reader ) { vostok::detail::unreferenced_parameter_helper( reader ); VOSTOK_UNREACHABLE_CODE( ); }

protected:
	/* 0x0000 */	/* interactive_object */
	/* 0x0108 */	/* boost::noncopyable */
	/* 0x0108 */	inventory_item::action_behaviour_type const	m_action_behaviuor;
	/* 0x010c */	inventory*								m_inventory;
	/* 0x0110 */	profile_slot_enum						m_slot_id;
	/* 0x0114 */	u16										m_amount;
	/* 0x0116 */	u16										m_dict_id;
}; // class inventory_item

STATIC_SIZE_ASSERT(inventory_item, 0x118);

typedef resources::resource_ptr< inventory_item, resources::unmanaged_intrusive_base > inventory_item_ptr;

} // namespace survarium

#endif // #ifndef INVENTORY_ITEM_H_INCLUDED
