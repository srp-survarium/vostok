////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATED_MODEL_INSTANCE_H_INCLUDED
#define ANIMATED_MODEL_INSTANCE_H_INCLUDED

// render::animated_model_instance_ptr / physics::animated_model_instance_ptr /
// damage_model_ptr value members
#include <vostok/render/facade/model.h>
#include <vostok/physics/model.h>
#include <vostok/game_core/damage_model.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace animation {
	class animation_player;
} // namespace animation
namespace collision {
	class animated_object;
} // namespace collision
} // namespace vostok

namespace survarium {

struct animated_model_instance : public resources::unmanaged_resource {

public:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	render::animated_model_instance_ptr		m_render_model;
	/* 0x010c */	physics::animated_model_instance_ptr	m_physics_model;
	/* 0x0110 */	damage_model_ptr						m_damage_model;
	/* 0x0114 */	collision::animated_object*				m_damage_collision;
	/* 0x0118 */	animation::animation_player*			m_animation_player;
}; // struct animated_model_instance

STATIC_SIZE_ASSERT(animated_model_instance, 0x120);

typedef resources::resource_ptr<
	animated_model_instance,
	resources::unmanaged_intrusive_base
> animated_model_instance_ptr;

} // namespace survarium

#endif // #ifndef ANIMATED_MODEL_INSTANCE_H_INCLUDED
