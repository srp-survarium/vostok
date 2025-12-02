////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include <vostok/game_core/profile_slot_enum.h>
#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/inventory_slot.h>

namespace survarium {

class inventory_holder;

class player_profile;		// sushi@TODO
class items_dictionary;
class victory_item_core;

class inventory : public resources::unmanaged_resource , public boost::noncopyable {
public:
												inventory					( );

			void								set_item					( profile_slot_enum slot, inventory_item_ptr const& item );

			void								set_holder					( inventory_holder* holder );
			void								unset_holder				( );
			inventory_holder&					holder						( );

			inventory_item_ptr&					item_in_slot				( profile_slot_enum slot );
	inline	inventory_item_ptr const&			item_in_slot				( profile_slot_enum slot ) const { /* no source */ }

			void								set_victory_item			( victory_item_core* item );
	inline	victory_item_core*					get_victory_item			( ) { /* no source */ }

			bool								action						( profile_slot_enum slot_id, bool key_down );

			profile_slot_enum					get_active_slot				( ) const;

			void								setup_from_profile			( player_profile& profile, items_dictionary const& dict );
			void								setup_demo_profile			( );
			void								unload_to_profile			( player_profile& profile, items_dictionary const& dict );

			void								remove						( );

	inline	void								serialize					( network_core::udp_match_packet& arg_0, u32 arg_1 ) const { /* no source */ }
			void								deserialize					( network_core::packet_reader& reader );

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* boost::noncopyable */
	/* 0x0108 */	inventory_slot			m_slots[19]; // sushi@TODO: Enum_MAX
	/* 0x0154 */	profile_slot_enum		m_active_slot;
	/* 0x0158 */	inventory_holder*		m_holder;
	/* 0x015c */	victory_item_core*		m_victory_item;
}; // class inventory

STATIC_SIZE_ASSERT(inventory, 0x160);

typedef resources::resource_ptr< inventory, resources::unmanaged_intrusive_base > inventory_ptr;

} // namespace survarium

#endif // #ifndef INVENTORY_H_INCLUDED
