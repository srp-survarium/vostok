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

struct player_profile;
class items_dictionary;
class victory_item_core;

class inventory : public resources::unmanaged_resource , private boost::noncopyable {
public:
												inventory					( );

			void								set_item					( const profile_slot_enum slot, inventory_item_ptr const& item );

			void								set_holder					( inventory_holder* holder );
			void								unset_holder				( );
	inline	inventory_holder&					holder						( ) { return *m_holder; }

	inline	inventory_item_ptr const&			item_in_slot				( const profile_slot_enum slot ) const { return m_slots[slot].item; }
	inline	inventory_item_ptr&					item_in_slot				( const profile_slot_enum slot ) { return m_slots[slot].item; }

			void								set_victory_item			( victory_item_core* item );
	inline	victory_item_core*					get_victory_item			( ) { return m_victory_item; }

			bool								action						( const profile_slot_enum slot_id, bool key_down );

	inline	profile_slot_enum					get_active_slot				( ) const { return m_active_slot; }

			void								setup_from_profile			( player_profile& profile, items_dictionary const& dict );
			void								setup_demo_profile			( );
			void								unload_to_profile			( player_profile& profile, items_dictionary const& dict );

			void								remove						( );

			void								serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const;
			void								deserialize					( network_core::packet_reader& reader );

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* boost::noncopyable */
	/* 0x0108 */	inventory_slot			m_slots[max_slots_count];
	/* 0x0154 */	profile_slot_enum		m_active_slot;
	/* 0x0158 */	inventory_holder*		m_holder;
	/* 0x015c */	victory_item_core*		m_victory_item;
}; // class inventory

STATIC_SIZE_ASSERT(inventory, 0x160);

typedef resources::resource_ptr< inventory, resources::unmanaged_intrusive_base > inventory_ptr;

} // namespace survarium

#endif // #ifndef INVENTORY_H_INCLUDED
