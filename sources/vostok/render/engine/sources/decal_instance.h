#ifndef VOSTOK_RENDER_ENGINE_DECAL_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_DECAL_INSTANCE_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/math_aabb.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/facade/decal_properties.h>

#include "material_effects.h"

namespace vostok {

namespace collision {

class geometry_instance;
class object;
struct space_partitioning_tree;

} // namespace collision

namespace render {

enum enum_render_stage_type;

class renderer_context;

struct decal_instance :
	public resource_intrusive_base,
	public boost::noncopyable
{
	decal_instance(
		collision::space_partitioning_tree*	tree,
		decal_properties const&				properties,
		u32 const							id
	);

	~decal_instance( );

	void set_properties( decal_properties const& in_properties );
	decal_properties const& get_properties( ) const;
	material_effects const& get_effects( ) const;

	u32 draw(
		renderer_context*		context,
		res_effect_ptr			opaque_geometry_mask_effect,
		enum_render_stage_type	stage_type
	);

	math::aabb get_aabb( ) const
	{
		return m_aabb;
	}

	bool is_occluded( ) const;
	void destroy_impl( ) const;

private:
	void render( renderer_context* context, enum_render_stage_type stage_type );
	void render_geometry( );
	void set_materail_effects( resources::unmanaged_resource_ptr const& in_ptr );
	void remove_collision( );

public:
	friend class								scene;
	friend class								stage_visibility;

private:
	decal_properties						m_properties;
	math::aabb								m_aabb;
	u32										m_id;
	collision::space_partitioning_tree* const	m_collision_tree;
	collision::geometry_instance*			m_collision_geometry;
	collision::object*						m_collision_object;
	u32										m_occlusion_info_index;
	float									m_draw_priority;
	bool									m_occluded;
};

STATIC_SIZE_ASSERT( decal_instance, 0x9C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_DECAL_INSTANCE_H_INCLUDED
