// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_AMBIENT_VOLUME_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_AMBIENT_VOLUME_H_INCLUDED
#include <boost/noncopyable.hpp>
#include <vostok/math_aabb.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/facade/ambient_volume_properties.h>

namespace vostok {
namespace render {

struct ambient_volume :
	public resource_intrusive_base,
	public boost::noncopyable
{
	ambient_volume( ambient_volume_properties const& properties, u32 const id );
	~ambient_volume( ) { }

	void set_properties( ambient_volume_properties const& in_properties );
	bool is_occluded( ) const;

	void destroy_impl( ) const { }

	math::aabb get_aabb( ) const
	{
		return m_aabb;
	}

	ambient_volume_properties	m_properties;
	math::aabb					m_aabb;
	u32							m_id;
	u32							m_occlusion_info_index;
	bool						m_occluded;
};

STATIC_SIZE_ASSERT( ambient_volume, 0x70 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_AMBIENT_VOLUME_H_INCLUDED
