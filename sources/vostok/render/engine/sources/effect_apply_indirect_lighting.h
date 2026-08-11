#ifndef VOSTOK_RENDER_ENGINE_EFFECT_APPLY_INDIRECT_LIGHTING_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_APPLY_INDIRECT_LIGHTING_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_apply_indirect_lighting : public effect_descriptor {
public:
	enum {
		apply_indirect_diffuse_pass,
		apply_indirect_specular_pass
	};

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_apply_indirect_lighting, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_APPLY_INDIRECT_LIGHTING_H_INCLUDED
