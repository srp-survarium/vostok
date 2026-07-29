////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_SURFACE_INSTANCE_H_INCLUDED
#define RENDER_RENDER_SURFACE_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::render::render_model_instance_impl;
class vostok::render::render_surface;

namespace vostok {
namespace render {

struct render_surface_instance {
	inline			render_surface_instance	( ) { /* no source */ }

			void	set_constants			( );

			bool	is_occluded				( ) const;

	/* 0x0000 */	render_surface*					m_render_surface;
	/* 0x0004 */	float4x4*						m_transform;
	/* 0x0008 */	render_model_instance_impl*		m_parent;
	/* 0x000c */	u32								m_occlusion_info_index;
	/* 0x0010 */	float							m_dynamic_screen_factor;
	/* 0x0014 */	u32								m_flags;
	/* 0x0018 */	u8								m_shader_lod_index;
	/* 0x0019 */	bool							m_occluded;
}; // struct render_surface_instance

STATIC_SIZE_ASSERT(render_surface_instance, 0x1C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_SURFACE_INSTANCE_H_INCLUDED
