#ifndef VOSTOK_RENDER_ENGINE_EFFECT_APPLY_DECAL_NORMALS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_APPLY_DECAL_NORMALS_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_apply_decal : public effect_descriptor {
public:
	enum {
		normals_blend_pass,
		normals_write_pass,
		diffuse_write_pass
	};

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_apply_decal, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_APPLY_DECAL_NORMALS_H_INCLUDED
