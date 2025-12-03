////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_WORLD_OBJECT_H_INCLUDED
#define GAME_WORLD_OBJECT_H_INCLUDED
// sushi@TODO: SHOULD BE MOVED TO GAME FROM GAME_CORE
namespace survarium {

class inventory_item;

class game_world_object;
typedef resources::resource_ptr< game_world_object, resources::unmanaged_intrusive_base > game_world_object_ptr; 

class game_world_object : public resources::unmanaged_resource {
public:
	virtual	inventory_item*				owner						( ) { /* no source */ }
	virtual	inventory_item const*		owner						( ) const;

	virtual	void						serialize					( network_core::udp_match_packet& packet ) const	= 0;
	virtual	void						deserialize					( network_core::packet_reader& reader )				= 0;

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	game_world_object_ptr	next;
}; // class game_world_object

STATIC_SIZE_ASSERT(game_world_object, 0x110);

} // namespace survarium

#endif // #ifndef GAME_WORLD_OBJECT_H_INCLUDED
