// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_EFFECT_HIZ_OCCLUSION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_HIZ_OCCLUSION_H_INCLUDED
#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_hiz_occlusion : public effect_descriptor {
public:
	enum {
		hiz_render_debug_geometry_pass,
		hiz_render_debug_geometry_invisible_pass,
		hiz_copy_scene_depth_pass,
		hiz_occluders_depth_pass,
		hiz_downsample_depth_pass,
		hiz_merge_mip_pass,
		hiz_fill_culling_results_buffer_pass,
		hiz_copy_to_lockable_render_target
	};

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_hiz_occlusion, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_HIZ_OCCLUSION_H_INCLUDED
