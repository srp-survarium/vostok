#ifndef VOSTOK_RENDER_ENGINE_EFFECT_DECAL_MASK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_DECAL_MASK_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_decal_mask : public effect_descriptor {
public:
	enum {
		specific_type,
		all_types
	};

	effect_decal_mask( ) { }
	virtual ~effect_decal_mask( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_decal_mask, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_DECAL_MASK_H_INCLUDED
