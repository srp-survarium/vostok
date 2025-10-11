////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

namespace survarium {

// STATE[STUB]
// vostok::resources::resource_ptr<survarium::inventory_item,vostok::resources::unmanaged_intrusive_base>& survarium::inventory::item_in_slot(const survarium::profile_slot_enum)
vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base>& inventory::item_in_slot( profile_slot_enum slot )
{
}

// STATE[STUB]
// survarium::profile_slot_enum survarium::inventory::get_active_slot() const
profile_slot_enum inventory::get_active_slot( ) const
{
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

#endif // #ifndef INVENTORY_H_INCLUDED
