////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PACKET_READER_INLINE_H_INCLUDED
#define PACKET_READER_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[STUB]
// void vostok::network_core::packet_reader::r(void*, unsigned int, unsigned int)
void packet_reader::r( void* destination, u32 size )
{
	// LOCALS
	// u32 							destination_size
	// ******

	// FUNCTION BODY[0x8e5a0]: 6
	// 1
	// 2
	// 3
	// 4
	// <0x8e5a0>|0x000|0x000:'25'
	// <0x8e5af>|0x00f|0x00f:'26'
	// ******
}

// STATE[STUB]
// unsigned char vostok::network_core::packet_reader::r<unsigned char>()
u8 packet_reader::r<u8>( )
{
	return 0;
	// FUNCTION BODY[0x8e950]: 3
	// 1
	// <0x8e950>|0x000|0x000:'33'
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
		fixed_vector<std::pair<enum survarium::game_action_id,enum survarium::player_input_handler::action_state_enum>,32>
		actions_type;

	typedef
		survarium::base_project::resolve_link_object*
		iterator_type;

	typedef
		survarium::inventory_item_instance*
		iterator_type;

	typedef
		survarium::scheduler::record*
		iterator_type;

	typedef
		void**
		iterator_type;

	// ******

} // namespace network_core
} // namespace vostok

#endif // #ifndef PACKET_READER_INLINE_H_INCLUDED
