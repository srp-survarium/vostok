////////////////////////////////////////////////////////////////////////////
//	Created		: 05.05.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATED_MODEL_INSTANCE_H_INCLUDED
#define ANIMATED_MODEL_INSTANCE_H_INCLUDED

#include <vostok/render/facade/model.h>
#include <vostok/physics/model.h>
#include <vostok/collision/animated_object.h>
#include <vostok/animation/animation_player.h>
#include <vostok/animation/animation_player.h>

namespace vostok {
namespace editor {

struct animated_model_instance : public resources::unmanaged_resource 
{
	render::animated_model_instance_ptr		m_render_model;
	physics::animated_model_instance_ptr	m_physics_model;
	collision::animated_object*				m_damage_collision;
	animation::animation_player*			m_animation_player;
}; // struct animated_model_instance

typedef	resources::resource_ptr<
			animated_model_instance,
			resources::unmanaged_intrusive_base >
		animated_model_instance_ptr;

} // namespace editor
} // namespace vostok

#endif // #ifndef ANIMATED_MODEL_INSTANCE_H_INCLUDED