////////////////////////////////////////////////////////////////////////////
//	Created		: 03.11.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////


#pragma make_public (vostok::configs::lua_config_value)
#pragma make_public (vostok::math::color)

#pragma make_public (vostok::math::float3)
#pragma make_public (vostok::math::float4x4)
#pragma make_public (vostok::math::int2)
#pragma make_public (vostok::math::float2)
#pragma make_public (vostok::math::aabb)

#include <vostok/collision/object.h>
#pragma make_public (vostok::collision::object)

#include <vostok/render/facade/editor_renderer.h>
#include <vostok/render/facade/debug_renderer.h>
#include <vostok/render/world.h>
#pragma make_public (vostok::render::editor::renderer)
#pragma make_public (vostok::render::debug::renderer)
#pragma make_public (vostok::render::world)
#pragma make_public (vostok::resources::unmanaged_resource)

#pragma make_public (vostok::render::base_scene)
#pragma make_public (vostok::render::base_scene_view)
#pragma make_public (vostok::render::base_output_window)
 
#pragma make_public (vostok::editor_base::allocator_type)
#pragma make_public (vostok::collision::space_partitioning_tree)
#pragma make_public (vostok::collision::ray_object_result)

#include <vostok/animation/camera_follower.h>
#pragma make_public	(vostok::camera_effector)

#pragma make_public(vostok::editor_base::property_holder)
#pragma make_public(vostok::editor_base::editor_intrusive_base)
#pragma make_public(vostok::threading::single_threading_policy)
#pragma make_public(vostok::editor_base::property_holder_collection)
#pragma make_public(vostok::editor_base::property_holder_holder)

