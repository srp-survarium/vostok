#ifndef VOSTOK_RENDER_ENGINE_EFFECT_FILL_ENVIRONMENT_PROBE_FACE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_FILL_ENVIRONMENT_PROBE_FACE_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_fill_environment_probe_face : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;

	effect_fill_environment_probe_face( ) { }
	virtual ~effect_fill_environment_probe_face( ) { }
};

STATIC_SIZE_ASSERT( effect_fill_environment_probe_face, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_FILL_ENVIRONMENT_PROBE_FACE_H_INCLUDED
