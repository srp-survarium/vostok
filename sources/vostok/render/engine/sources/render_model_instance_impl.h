#ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_INSTANCE_IMPL_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_MODEL_INSTANCE_IMPL_H_INCLUDED

#include <vostok/render/facade/model.h>
#include "render_collision_object.h"

namespace vostok {
namespace render {

struct render_surface_instance;

class render_model_instance_impl : public render_model_instance {
public:
	render_model_instance_impl( );

	virtual void set_constants( )
	{
	}

	virtual math::aabb get_aabb( ) = 0;

	virtual void get_surfaces(
		float4x4 const*,
		float3 const*,
		vector< render_surface_instance* >&,
		bool,
		u8,
		u32
	)
	{
	}

	virtual void set_transform( float4x4 const& transform )
	{
		m_transform = transform;
	}

	float4x4 const& transform( ) const
	{
		return m_transform;
	}

	render_collision_object< render_model_instance_impl >	m_collision_object;

protected:
	float4x4	m_transform;
};

STATIC_SIZE_ASSERT( render_model_instance_impl, 0x188 );

typedef resources::resource_ptr<
	render_model_instance_impl,
	resources::unmanaged_intrusive_base
> render_model_instance_impl_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_INSTANCE_IMPL_H_INCLUDED
