#ifndef VOSTOK_RENDER_ENGINE_EFFECT_COPY_IMAGE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_COPY_IMAGE_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_copy_image : public effect_descriptor {
public:
	enum
	{
		copy_rewrite  = 0,
		copy_additive,
	};

	effect_copy_image( ) { }
	virtual ~effect_copy_image( ) { }

	virtual void compile(
		effect_compiler&,
		custom_config_value const&
	) override;
};

STATIC_SIZE_ASSERT( effect_copy_image, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_COPY_IMAGE_H_INCLUDED
