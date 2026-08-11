#include "pch.h"
#include <vostok/collision/api.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>

#include "environment_probe.h"

namespace vostok {
namespace render {

environment_probe::environment_probe(
	collision::space_partitioning_tree*	tree,
	environment_probe_properties const&	properties,
	u32 const							id
) :
	m_properties				( ),
	m_aabb						( math::create_identity_aabb( ) ),
	m_id						( id ),
	m_num_mips					( 1 ),
	m_collision_tree			( tree ),
	m_collision_geometry		( 0 ),
	m_collision_object			( 0 ),
	m_occlusion_info_index		( u32(-1) ),
	m_occluded					( false )
{
	set_properties(properties);
}

environment_probe::~environment_probe( )
{
	remove_collision();
}

void environment_probe::remove_collision( )
{
	if (m_collision_tree && m_collision_object)
		m_collision_tree->erase(m_collision_object);

	collision::delete_object(g_allocator, m_collision_object);
	collision::delete_geometry_instance(g_allocator, m_collision_geometry);
}

bool environment_probe::is_occluded( ) const
{
	return options::ref( ).current.m_use_hiz_occlusion_culling && m_occluded;
}

static u32 calc_mip_map_count( u32 width )
{
	return u32(log(double(width)) / log(2.0)) + 1;
}

void environment_probe::set_properties(
	environment_probe_properties const& in_properties
)
{
	m_properties = in_properties;

	m_num_mips = calc_mip_map_count(in_properties.cubemap_resolution);
	float4x4 new_transform;
	if (in_properties.texture_invalidated)
	{
		m_texture = 0;
		m_texture_depth = 0;

		m_texture = resource_manager::ref().create_texture(m_properties.texture_name.c_str(), 0, 0, false, true, true, u32(-1));

		if (in_properties.with_shadows)
		{
			fixed_string<260> depth_texture_name;
			depth_texture_name.assignf("%s_depth", m_properties.texture_name.c_str());

			m_texture_depth = resource_manager::ref().create_texture(depth_texture_name.c_str(), 0, 0, false, true, true, u32(-1));
		}
	}

	remove_collision();

	float3 probe_scale3;

	if (m_properties.geometry == 0)
	{
		m_collision_geometry = &*collision::new_sphere_geometry_instance(g_allocator, math::float4x4().identity());
		m_collision_object = &*collision::new_collision_object(g_allocator, 1, m_collision_geometry, this);
		new_transform = math::create_translation(in_properties.location);
		probe_scale3 = float3(in_properties.radius, in_properties.radius, in_properties.radius);
		new_transform.set_scale(probe_scale3);
		m_collision_tree->insert(m_collision_object, new_transform);
	}
	else
	{
		m_collision_geometry = &*collision::new_box_geometry_instance(g_allocator, math::float4x4().identity());
		m_collision_object = &*collision::new_collision_object(g_allocator, 1, m_collision_geometry, this);
		new_transform = m_properties.transform;
		m_collision_tree->insert(m_collision_object, new_transform);

		probe_scale3 = new_transform.get_scale();

		float const max_scale = math::max(probe_scale3.x,
			math::max(probe_scale3.y, probe_scale3.z));
		m_properties.location = new_transform.c.xyz();
		m_properties.radius = max_scale * 0.75f;
	}

	m_aabb = math::create_identity_aabb();
	m_aabb.modify(new_transform);
}

} // namespace render
} // namespace vostok
