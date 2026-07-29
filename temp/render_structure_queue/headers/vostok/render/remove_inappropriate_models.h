////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_REMOVE_INAPPROPRIATE_MODELS_H_INCLUDED
#define RENDER_REMOVE_INAPPROPRIATE_MODELS_H_INCLUDED

/* INCLUDES */
class vostok::math::frustum;
class vostok::render::renderer_context;

/* FORWARD REFS */
class vostok::render::render_surface_instance;

namespace vostok {
namespace render {

struct remove_inappropriate_models {
	inline			remove_inappropriate_models	(
						renderer_context*		arg_0,
						math::frustum&			arg_1,
						float3 const&			arg_2,
						float					arg_3
					) { /* no source */ }

			bool	operator()					( render_surface_instance* in_model );

	/* 0x0000 */	renderer_context*		m_context;
	/* 0x0004 */	float3					m_sun_dir;
	/* 0x0010 */	float					m_cascade_size;
	/* 0x0014 */	math::frustum*			m_frustum;
}; // struct remove_inappropriate_models

STATIC_SIZE_ASSERT(remove_inappropriate_models, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_REMOVE_INAPPROPRIATE_MODELS_H_INCLUDED
