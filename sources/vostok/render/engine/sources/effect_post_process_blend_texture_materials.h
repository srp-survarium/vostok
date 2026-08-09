#ifndef VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_BLEND_TEXTURE_MATERIALS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_BLEND_TEXTURE_MATERIALS_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_post_process_blend_texture_materials : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	custom_config
	) override;

	effect_post_process_blend_texture_materials( ) { }
	virtual ~effect_post_process_blend_texture_materials( ) { }
};

STATIC_SIZE_ASSERT( effect_post_process_blend_texture_materials, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_POST_PROCESS_BLEND_TEXTURE_MATERIALS_H_INCLUDED
