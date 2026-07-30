#ifndef VOSTOK_RENDER_ENGINE_REMOVE_INAPPROPRIATE_MODELS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_REMOVE_INAPPROPRIATE_MODELS_H_INCLUDED

#include <vostok/math_float3.h>

namespace vostok {

namespace math {

class frustum;

} // namespace math

namespace render {

class renderer_context;
struct render_surface_instance;

struct remove_inappropriate_models {
	remove_inappropriate_models(
		renderer_context*	context,
		math::frustum&		frustum,
		float3 const&		sun_direction,
		float				cascade_size
	) :
		m_context		( context ),
		m_sun_dir		( sun_direction ),
		m_cascade_size	( cascade_size ),
		m_frustum		( &frustum )
	{
	}

	bool operator()( render_surface_instance* in_model );

	renderer_context*	m_context;
	float3				m_sun_dir;
	float				m_cascade_size;
	math::frustum*		m_frustum;
};

STATIC_SIZE_ASSERT( remove_inappropriate_models, 0x18 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_REMOVE_INAPPROPRIATE_MODELS_H_INCLUDED
