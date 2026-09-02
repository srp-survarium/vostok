// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef GAME_WORLD_OBJECT_H_INCLUDED
#define GAME_WORLD_OBJECT_H_INCLUDED

namespace vostok {
namespace network_core {
	class udp_match_packet;
	class packet_reader;
}
}

namespace survarium {

class inventory_item;

class game_world_object;
typedef resources::resource_ptr< game_world_object, resources::unmanaged_intrusive_base > game_world_object_ptr;

class game_world_object : public resources::unmanaged_resource {
public:
	virtual	inventory_item const*		owner						( ) const { return NULL; }
	virtual	inventory_item*				owner						( ) { return NULL; }

	virtual	void						serialize					( network_core::udp_match_packet& packet ) const	= 0;
	virtual	void						deserialize					( network_core::packet_reader& reader )				= 0;

public:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	game_world_object_ptr	next;
}; // class game_world_object

STATIC_SIZE_ASSERT(game_world_object, 0x110);

typedef intrusive_list<
	game_world_object,
	game_world_object_ptr,
	&game_world_object::next,
	threading::single_threading_policy,
	no_size_policy,
	no_debug_policy > game_world_object_list;

} // namespace survarium

#endif // #ifndef GAME_WORLD_OBJECT_H_INCLUDED
