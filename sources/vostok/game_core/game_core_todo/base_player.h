////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_PLAYER_H_INCLUDED
#define BASE_PLAYER_H_INCLUDED

namespace survarium {

// STATE[STUB]
// bool survarium::base_player::is_alive() const
bool base_player::is_alive( ) const
{
	return false;
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

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

} // namespace survarium

#endif // #ifndef BASE_PLAYER_H_INCLUDED
