#ifndef VOSTOK_RENDER_ENGINE_ENVIRONMENT_PROBE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_ENVIRONMENT_PROBE_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/math_aabb.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/facade/environment_probe_properties.h>

#include "res_texture.h"

namespace vostok {

namespace collision {

class geometry_instance;
class object;
struct space_partitioning_tree;

} // namespace collision

namespace render {

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

struct environment_probe :
	public resource_intrusive_base,
	public boost::noncopyable
{
	environment_probe(
		collision::space_partitioning_tree*	tree,
		environment_probe_properties const&	properties,
		u32									id
	);

	~environment_probe( );

	void set_properties( environment_probe_properties const& in_properties );
	bool is_occluded( ) const;

	void destroy_impl( ) const { }

	math::aabb get_aabb( ) const
	{
		return m_aabb;
	}

	void remove_collision( );

	environment_probe_properties				m_properties;
	math::aabb									m_aabb;
	res_texture_ptr								m_texture;
	res_texture_ptr								m_texture_depth;
	u32											m_id;
	u32											m_num_mips;
	collision::space_partitioning_tree* const	m_collision_tree;
	collision::geometry_instance*				m_collision_geometry;
	collision::object*							m_collision_object;
	u32											m_occlusion_info_index;
	bool										m_occluded;
};

STATIC_SIZE_ASSERT( environment_probe, 0x1B8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_ENVIRONMENT_PROBE_H_INCLUDED
