////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INVENTORY_ITEM_H_INCLUDED
#define INVENTORY_ITEM_H_INCLUDED

namespace survarium {

// STATE[STUB]
// void survarium::player_logic_base_state::~player_logic_base_state()
void player_logic_base_state::~player_logic_base_state( )
{
}

// STATE[STUB]
// void survarium::inventory_item::deserialize_game_world_object(vostok::network_core::packet_reader&)
void inventory_item::deserialize_game_world_object( vostok::network_core::packet_reader& reader )
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x9a263 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x9a263 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// ******

}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

	typedef
		long
		counter_type;

	typedef
		scheduler::record*
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

	typedef
		vostok::memory::multi_threading_single_size_allocator_policy<vostok::memory::single_size_buffer_allocator<128,vostok::threading::simple_lock>::node>::free_list_type
		free_list_type;

} // namespace survarium

#endif // #ifndef INVENTORY_ITEM_H_INCLUDED
