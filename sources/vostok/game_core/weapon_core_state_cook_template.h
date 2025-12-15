////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_H_INCLUDED
#define WEAPON_CORE_STATE_COOK_TEMPLATE_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

template < typename T >
class weapon_core_state_cook_template<T> : public resources::unmanaged_cook {
public:
	inline	explicit	weapon_core_state_cook_template		( ) { /* no source */ }
	virtual				~weapon_core_state_cook_template	( ) { /* no source */ }

public:
	virtual	mutable_buffer	allocate_resource		( resources::query_result_for_cook& arg_0, const_buffer arg_1, bool arg_2 ) override { /* no source */ }
	virtual	void			deallocate_resource		( void* arg_0 ) override { /* no source */ }

	virtual	void			create_resource			( resources::query_result_for_cook& arg_0, const_buffer arg_1, mutable_buffer arg_2 ) override { /* no source */ }
	virtual	void			destroy_resource		( resources::unmanaged_resource* arg_0 ) override { /* no source */ }

	inline	void			on_subresources_ready	( resources::queries_result& arg_0, mutable_buffer arg_1, weapon_state_creation_params const* arg_2 ) { /* no source */ }

			T*				new_object				(
								mutable_buffer							buffer,
								weapon_state_creation_params const*		params,
								resources::managed_resource_ptr const*	animations,
								u32										animations_count
							);
}; // class weapon_core_state_cook_template

STATIC_SIZE_ASSERT(weapon_core_state_cook_template<T>, 0x20);

} // namespace survarium

#include <vostok/game_core/weapon_core_state_cook_template_inline.h>

#endif // #ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_H_INCLUDED
