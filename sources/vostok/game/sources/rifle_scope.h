// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef RIFLE_SCOPE_H_INCLUDED
#define RIFLE_SCOPE_H_INCLUDED

#include <vostok/render/facade/model.h>
#include <vostok/resources_unmanaged_resource.h>

namespace survarium {

class rifle_scope : public resources::unmanaged_resource {
public:
						rifle_scope			(
							render::static_model_ptr const&		idle_scope,
							render::static_model_ptr const&		aimed_scope,
							const float		change_scope_factor,
							const bool		hide_weapon_on_aim,
							const float		fov_factor,
							const float		near_plane_factor
						);

	inline	render::static_model_ptr const&	idle_model			( ) const { return m_idle_scope; }
	inline	render::static_model_ptr const&	aimed_model			( ) const { return m_aimed_scope; }

	inline	float		change_scope_factor	( ) const { return m_change_scope_factor; }

	inline	bool		hide_weapon_on_aim	( ) const { return m_hide_weapon_on_aim; }

	inline	float		fov_factor			( ) const { return m_fov_factor; }
	inline	float		near_plane_factor	( ) const { return m_near_plane_factor; }


private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	render::static_model_ptr	m_idle_scope;
	/* 0x010c */	render::static_model_ptr	m_aimed_scope;
	/* 0x0110 */	const float					m_change_scope_factor;
	/* 0x0114 */	const float					m_fov_factor;
	/* 0x0118 */	const float					m_near_plane_factor;
	/* 0x011c */	const bool					m_hide_weapon_on_aim;
}; // class rifle_scope

STATIC_SIZE_ASSERT(rifle_scope, 0x120);

typedef resources::resource_ptr< rifle_scope, resources::unmanaged_resource > rifle_scope_ptr;

} // namespace survarium

#endif // #ifndef RIFLE_SCOPE_H_INCLUDED
