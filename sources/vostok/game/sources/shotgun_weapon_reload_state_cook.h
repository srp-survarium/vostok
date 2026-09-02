// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef SHOTGUN_WEAPON_RELOAD_STATE_COOK_H_INCLUDED
#define SHOTGUN_WEAPON_RELOAD_STATE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/detail_noncopyable.h>

namespace survarium {

struct weapon_state_creation_params;

class shotgun_weapon_reload_state_cook : public resources::unmanaged_cook , private core::noncopyable {
	typedef resources::unmanaged_cook super;

public:
								shotgun_weapon_reload_state_cook	( );
	virtual						~shotgun_weapon_reload_state_cook	( );

	virtual	mutable_buffer		allocate_resource					(
									resources::query_result_for_cook&		in_query,
									const_buffer							raw_file_data,
									bool									file_exist
								) override;
	virtual	void				deallocate_resource					( void* buffer ) override;
	virtual	void				create_resource						(
									resources::query_result_for_cook&		parent,
									const_buffer							raw_file_data,
									mutable_buffer							in_out_unmanaged_resource_buffer
								) override;
	virtual	void				destroy_resource					( resources::unmanaged_resource* resource ) override;

private:
			void				on_substates_ready					(
									resources::queries_result&				data,
									mutable_buffer							buffer,
									weapon_state_creation_params const*	params
								);
}; // class shotgun_weapon_reload_state_cook

STATIC_SIZE_ASSERT(shotgun_weapon_reload_state_cook, 0x20);

} // namespace survarium

#endif // #ifndef SHOTGUN_WEAPON_RELOAD_STATE_COOK_H_INCLUDED
