#ifndef VOSTOK_RENDER_FACADE_COMMON_TYPES_H_INCLUDED
#define VOSTOK_RENDER_FACADE_COMMON_TYPES_H_INCLUDED

namespace survarium {

class scaleform_render_command_queue;

} // namespace survarium

namespace vostok {
namespace render {

struct scene_configuration {
	inline scene_configuration( ) { }

	bool m_create_terrain			: 1;
	bool m_create_particle_world	: 1;
	bool m_has_clouds				: 1;
	bool m_create_speedtree_world	: 1;
	bool m_create_grass_world		: 1;
	bool m_sky_enabled				: 1;
	bool m_use_occlusion_culling	: 1;
};

STATIC_SIZE_ASSERT( scene_configuration, 0x1 );

struct output_window_configuration {
	inline output_window_configuration( ) { }

	void*										hwnd;
	u32											width;
	u32											height;
	bool										create_flash_renderer;
	bool										windowed;
	survarium::scaleform_render_command_queue*	scaleform_render_queue;
};

STATIC_SIZE_ASSERT( output_window_configuration, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_COMMON_TYPES_H_INCLUDED
