////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PROFILE_CHARACTER_PREVIEW_WEAPON_H_INCLUDED
#define PROFILE_CHARACTER_PREVIEW_WEAPON_H_INCLUDED

/* INCLUDES */
struct vostok::render::model_locator_item;
typedef vostok::resources::resource_ptr<vostok::render::skeleton_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::skeleton_model_instance_ptr;
typedef vostok::resources::resource_ptr<vostok::render::static_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::static_model_instance_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
class vostok::render::skeleton_model_instance;
class vostok::render::static_model_instance;
class vostok::resources::managed_resource;

namespace survarium {

struct profile_character::preview_weapon {
	inline		preview_weapon	( ) { /* no source */ }
	inline		~preview_weapon	( ) { /* no source */ }

public:
	/* 0x0000 */	render::skeleton_model_instance_ptr		m_model;
	/* 0x0004 */	render::static_model_instance_ptr		m_addon;
	/* 0x0008 */	resources::managed_resource_ptr			m_animation;
	/* 0x000c */	render::model_locator_item				m_addon_locator;
	/* 0x0070 */	bool									m_visible;
}; // struct profile_character::preview_weapon

STATIC_SIZE_ASSERT(profile_character::preview_weapon, 0x74);

} // namespace survarium

#endif // #ifndef PROFILE_CHARACTER_PREVIEW_WEAPON_H_INCLUDED
