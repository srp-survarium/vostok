////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef CIRCULAR_BUFFER_INLINE_H_INCLUDED
#define CIRCULAR_BUFFER_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
// void survarium::circular_buffer<survarium::client_player_history_item>::~circular_buffer<survarium::client_player_history_item>()
void circular_buffer<client_player_history_item>::~circular_buffer<client_player_history_item>( )
{
	// FUNCTION BODY
	// <0x8efc0>|0x000|0x000:'25'
	// <0x8efd0>|0x010|0x010:'26'
	// 1
	// <0x8efe3>|0x023|0x013:'28'
	// ******
}

// STATE[STUB]
// survarium::client_player_history_item& survarium::circular_buffer<survarium::client_player_history_item>::new_item()
client_player_history_item& circular_buffer<client_player_history_item>::new_item( )
{
	// FUNCTION BODY
	// <0x8f000>|0x000|0x000:'40'
	// <0x8f01e>|0x01e|0x01e:'41'
	// <0x8f021>|0x021|0x003:'42'
	// 1
	// <0x8f036>|0x036|0x015:'44'
	// <0x8f046>|0x046|0x010:'45'
	// ******
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

	typedef
		char[32]
		account_name_type;

	typedef
		inventory_item_instance*
		iterator_type;

	typedef
		scheduler::record*
		iterator_type;

	typedef
		void**
		iterator_type;

	typedef
		vostok::collision::bone_collision_data const*
		iterator_type;

	typedef
		vostok::collision::bone_collision_data*
		iterator_type;

	typedef
		vostok::fixed_vector<std::pair<enum game_action_id,enum player_input_handler::action_state_enum>,32>
		actions_type;

} // namespace survarium

#endif // #ifndef CIRCULAR_BUFFER_INLINE_H_INCLUDED
