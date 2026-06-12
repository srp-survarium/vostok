////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATED_MODEL_INSTANCE_H_INCLUDED
#define ANIMATED_MODEL_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::animation::animation_player;
class vostok::collision::animated_object;
class vostok::resources::unmanaged_resource;
typedef vostok::resources::resource_ptr<survarium::damage_model,vostok::resources::unmanaged_intrusive_base>
	survarium::damage_model_ptr;
typedef vostok::resources::resource_ptr<vostok::physics::animated_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::physics::animated_model_instance_ptr;
typedef vostok::resources::resource_ptr<vostok::render::animated_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::animated_model_instance_ptr;
class vostok::physics::animated_model_instance;
class vostok::render::animated_model_instance;
class survarium::damage_model;

namespace survarium {

struct animated_model_instance : public resources::unmanaged_resource {
	inline		animated_model_instance	( ) { /* no source */ }
	virtual		~animated_model_instance( ) { /* no source */ }

public:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	render::animated_model_instance_ptr		m_render_model;
	/* 0x010c */	physics::animated_model_instance_ptr	m_physics_model;
	/* 0x0110 */	damage_model_ptr						m_damage_model;
	/* 0x0114 */	collision::animated_object*				m_damage_collision;
	/* 0x0118 */	animation::animation_player*			m_animation_player;
}; // struct animated_model_instance

STATIC_SIZE_ASSERT(animated_model_instance, 0x120);

} // namespace survarium

#endif // #ifndef ANIMATED_MODEL_INSTANCE_H_INCLUDED
