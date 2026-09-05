// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_CORE_SHOTGUN_RELOAD_STATE_COOK_H_INCLUDED
#define WEAPON_CORE_SHOTGUN_RELOAD_STATE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

struct weapon_state_creation_params;


class weapon_core_shotgun_reload_state_cook : public resources::unmanaged_cook {
public:
	typedef resources::unmanaged_cook super;

	// sushi@TODO: Verify core ctor expansion; non-reuse/self-registration follows sibling state cooks, not an observed core caller.
	inline	explicit	weapon_core_shotgun_reload_state_cook	( ) :
							resources::unmanaged_cook	( resources::weapon_shotgun_reload_state_class, reuse_false, use_current_thread_id, use_current_thread_id )
	{
		resources::register_cook( this );
	}

	virtual				~weapon_core_shotgun_reload_state_cook	( );

public:
	virtual	mutable_buffer	allocate_resource				( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist ) override;
	virtual	void			deallocate_resource				( void* buffer ) override;

	virtual	void			create_resource					( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer ) override;
	virtual	void			destroy_resource				( resources::unmanaged_resource* resource ) override;

private:
			void			on_subresources_ready			( resources::queries_result& data, mutable_buffer buffer, weapon_state_creation_params const* params );

private:
	/* 0x0000 */	/* resources::unmanaged_cook */
}; // class weapon_core_shotgun_reload_state_cook

STATIC_SIZE_ASSERT(weapon_core_shotgun_reload_state_cook, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_SHOTGUN_RELOAD_STATE_COOK_H_INCLUDED
