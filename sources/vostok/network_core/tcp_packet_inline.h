////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef TCP_PACKET_INLINE_H_INCLUDED
#define TCP_PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[STUB]
// void vostok::network_core::tcp_packet::~tcp_packet()
void tcp_packet::~tcp_packet( )
{
	// FUNCTION BODY
	// <0xa72b0>|0x000|0x000:'21'
	// 1
	// <0xa72b6>|0x006|0x006:'23'
	// <0xa72b9>|0x009|0x003:'24'
	// 1
	// ******
}

	// TYPEDEFS
	typedef
		char[32]
		account_name_type;

	typedef
		collision::bone_collision_data const*
		iterator_type;

	typedef
		collision::bone_collision_data*
		iterator_type;

	typedef
		survarium::base_project::resolve_link_object*
		iterator_type;

	typedef
		survarium::inventory_item_instance*
		iterator_type;

	typedef
		survarium::scheduler::record*
		iterator_type;

	// ******

} // namespace network_core
} // namespace vostok

#endif // #ifndef TCP_PACKET_INLINE_H_INCLUDED
