// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_EFFECT_FSTAGE_SKY_MATERIALS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_FSTAGE_SKY_MATERIALS_H_INCLUDED
#include "effect_material_base.h"

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_fstage_sky_materials : public effect_material_base {
	enum {
		atmosphere_fog,
		sky_multiply
	};

public:
	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;

};

STATIC_SIZE_ASSERT( effect_fstage_sky_materials, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_FSTAGE_SKY_MATERIALS_H_INCLUDED
