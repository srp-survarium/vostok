////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef COLLISION_GEOMETRY_INLINE_H_INCLUDED
#define COLLISION_GEOMETRY_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
// void survarium::collision_geometry::query_objects_by_type<survarium::usable_object>(vostok::vectora<survarium::usable_object *>&, survarium::usable_object* (survarium::collision_geometry_subscriber::*)())
void collision_geometry::query_objects_by_type<usable_object>( vostok::vectora<usable_object *>& results, usable_object* (collision_geometry_subscriber::*) functor_ptr )
{
	// LOCALS
	// usable_object* 				result
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7e13e }, type_index: TypeIndex(0x477d6) })
	// ******

	// FUNCTION BODY
	// <0x8f125>|0x000|0x000:'15'
	// 1
	// <0x8f134>|0x00f|0x00f:'17'
	// <0x8f146>|0x021|0x012:'18'
	// <0x8f14a>|0x025|0x004:'19'
	// 1
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

#endif // #ifndef COLLISION_GEOMETRY_INLINE_H_INCLUDED
