////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_PROJECT_H_INCLUDED
#define BASE_PROJECT_H_INCLUDED

namespace survarium {

// STATE[STUB]
// void survarium::base_project::register_named_object(char const*, survarium::base_game_object*)
void base_project::register_named_object( pcstr name, base_game_object* obj )
{
}

// STATE[STUB]
// void survarium::base_project::register_object_to_resolve(survarium::link_resolver*, vostok::configs::binary_config_value)
void base_project::register_object_to_resolve( link_resolver* obj, vostok::configs::binary_config_value cfg )
{
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

	typedef
		scheduler::record*
		iterator_type;

	typedef
		void const**
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
		vostok::resources::creation_request*
		iterator_type;

	typedef
		vostok::resources::request*
		iterator_type;

	typedef
		vostok::resources::resource_ptr<damage_zone,vostok::resources::unmanaged_intrusive_base>*
		iterator_type;

	typedef
		vostok::resources::resource_ptr<ladder,vostok::resources::unmanaged_intrusive_base>*
		iterator_type;

	typedef
		zone_group::zone_wrapper*
		iterator_type;

} // namespace survarium

#endif // #ifndef BASE_PROJECT_H_INCLUDED
