#ifndef VOSTOK_RENDER_ENGINE_DEPTH_ACCUMULATE_MATERIAL_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_DEPTH_ACCUMULATE_MATERIAL_EFFECT_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class depth_accumulate_material_effect : public effect_material_base {
public:
	depth_accumulate_material_effect( ) { }
	virtual ~depth_accumulate_material_effect( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( depth_accumulate_material_effect, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_DEPTH_ACCUMULATE_MATERIAL_EFFECT_H_INCLUDED
