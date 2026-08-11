#ifndef VOSTOK_RENDER_ENGINE_EFFECT_WET_SURFACE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_WET_SURFACE_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_wet_surface : public effect_descriptor {
public:
	enum {
		enum_normal_modify_pass,
		enum_normal_write_pass,
		enum_color_modify_pass,
		enum_color_write_pass,
		enum_copy
	};

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_wet_surface, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_WET_SURFACE_H_INCLUDED
