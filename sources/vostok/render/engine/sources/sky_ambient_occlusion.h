#ifndef VOSTOK_RENDER_ENGINE_SKY_AMBIENT_OCCLUSION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SKY_AMBIENT_OCCLUSION_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/math_aabb.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/facade/sky_ambient_occlusion_properties.h>

#include "res_texture.h"

namespace vostok {
namespace render {

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

struct sky_ambient_occlusion :
	public resource_intrusive_base,
	public boost::noncopyable
{
	sky_ambient_occlusion(
		sky_ambient_occlusion_properties const&	properties,
		u32 const								id
	);

	~sky_ambient_occlusion( );

	void set_properties( sky_ambient_occlusion_properties const& in_properties );

	bool is_occluded( ) const
	{
		return m_occluded;
	}

	void destroy_impl( ) const { }

	math::aabb get_aabb( ) const
	{
		return m_aabb;
	}

	sky_ambient_occlusion_properties	m_properties;
	math::aabb							m_aabb;
	res_texture_ptr						m_texture;
	u32									m_id;
	u32									m_occlusion_info_index;
	bool								m_occluded;
};

STATIC_SIZE_ASSERT( sky_ambient_occlusion, 0x158 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SKY_AMBIENT_OCCLUSION_H_INCLUDED
