////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RIFLE_SCOPE_H_INCLUDED
#define RIFLE_SCOPE_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;
typedef vostok::resources::resource_ptr<vostok::render::static_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::static_model_instance_ptr;
class vostok::render::static_model_instance;

namespace survarium {

class rifle_scope : public resources::unmanaged_resource {
public:
						rifle_scope			(
							render::static_model_instance_ptr const&	idle_scope,
							render::static_model_instance_ptr const&	aimed_scope,
							const float		change_scope_factor,
							const bool		hide_weapon_on_aim,
							const float		fov_factor,
							const float		near_plane_factor
						);

	inline	render::static_model_instance_ptr const&	idle_model			( ) const { /* no source */ }
	inline	render::static_model_instance_ptr const&	aimed_model			( ) const { /* no source */ }

	inline	float		change_scope_factor	( ) const { /* no source */ }

	inline	bool		hide_weapon_on_aim	( ) const { /* no source */ }

	inline	float		fov_factor			( ) const { /* no source */ }
	inline	float		near_plane_factor	( ) const { /* no source */ }

	virtual				~rifle_scope		( ) { /* no source */ }

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	render::static_model_instance_ptr		m_idle_scope;
	/* 0x010c */	render::static_model_instance_ptr		m_aimed_scope;
	/* 0x0110 */	const float								m_change_scope_factor;
	/* 0x0114 */	const float								m_fov_factor;
	/* 0x0118 */	const float								m_near_plane_factor;
	/* 0x011c */	const bool								m_hide_weapon_on_aim;
}; // class rifle_scope

STATIC_SIZE_ASSERT(rifle_scope, 0x120);

} // namespace survarium

#endif // #ifndef RIFLE_SCOPE_H_INCLUDED
