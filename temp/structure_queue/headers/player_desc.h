////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_DESC_H_INCLUDED
#define PLAYER_DESC_H_INCLUDED

/* INCLUDES */
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::resources::unmanaged_resource;

namespace survarium {

struct player_desc {
	inline		player_desc	( ) { /* no source */ }
	inline		~player_desc( ) { /* no source */ }

public:
	/* 0x0000 */	resources::unmanaged_resource_ptr		player;
	/* 0x0004 */	bool									is_connected;
}; // struct player_desc

STATIC_SIZE_ASSERT(player_desc, 0x8);

} // namespace survarium

#endif // #ifndef PLAYER_DESC_H_INCLUDED
