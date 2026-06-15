////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_DESC_H_INCLUDED
#define PLAYER_DESC_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace survarium {

struct player_desc {
	// STATE[STUB]
	// addressed carcass from the network_client.h dump (the type's original
	// home, defined just above class network_client)
	inline		player_desc	( )
	{
		// FUNCTION BODY[0x92180]: 0
		// <0x92180>|0x000|+0x00c:'39'	{
		// <0x9218c>|0x00c|      :'40'	}
		// ******
	}
	inline		~player_desc( ) { /* no source */ }

public:
	/* 0x0000 */	resources::unmanaged_resource_ptr		player;
	/* 0x0004 */	bool									is_connected;
}; // struct player_desc

STATIC_SIZE_ASSERT(player_desc, 0x8);

} // namespace survarium

#endif // #ifndef PLAYER_DESC_H_INCLUDED
