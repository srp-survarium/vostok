#ifndef VOSTOK_RENDER_ENGINE_RENDER_SURFACE_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_SURFACE_INSTANCE_H_INCLUDED

namespace vostok {
namespace render {

class render_model_instance_impl;
class render_surface;

enum render_surface_instance_flags {
	fully_invisible		= 0x0,
	visible_flag		= 0x1,
	cast_shadow_flag	= 0x2,
};

struct render_surface_instance {
	render_surface_instance( ) { }

	void set_constants( );
	bool is_occluded( ) const;

	render_surface*				m_render_surface;
	float4x4*					m_transform;
	render_model_instance_impl*	m_parent;
	u32							m_occlusion_info_index;
	float						m_dynamic_screen_factor;
	u32							m_flags;
	u8							m_shader_lod_index;
	bool						m_occluded;
};

STATIC_SIZE_ASSERT( render_surface_instance, 0x1C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_SURFACE_INSTANCE_H_INCLUDED
