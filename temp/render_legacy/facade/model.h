////////////////////////////////////////////////////////////////////////////
//	Created		: 10.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_VISUAL_H_INCLUDED
#define VOSTOK_RENDER_BASE_VISUAL_H_INCLUDED

#include <vostok/collision/geometry_instance.h>
#include <vostok/physics/collision_shapes.h>

// REMAINDER: terrain_model_instance has NO counterpart in the new tree and
// ZERO target-symbol evidence (terrain facade was restructured for v0.100b).
// Everything else in this header was ported / is represented canonically:
// render_model_instance & friends -> sources/vostok/render/facade/model.h,
// speedtree_tree_base / speedtree_instance ->
// sources/vostok/render/engine/sources/speedtree_{tree_base,instance}.h.

namespace vostok {
namespace render {

class terrain_model_instance : public resources::unmanaged_resource
{
public:
	render::render_model_instance_ptr		m_render_model;
	collision::geometry_instance*			m_collision_geom;
	physics::bt_collision_shape_ptr			m_bt_collision_shape;
};

typedef	resources::resource_ptr<
	terrain_model_instance,
	resources::unmanaged_intrusive_base
> terrain_model_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_BASE_VISUAL_H_INCLUDED
