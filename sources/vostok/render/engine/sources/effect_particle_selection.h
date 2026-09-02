// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_EFFECT_PARTICLE_SELECTION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_PARTICLE_SELECTION_H_INCLUDED
#include "effect_material_base.h"

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_particle_selection : public effect_material_base {
public:
	enum
	{
		billboard		= 0,
		billboard_subuv,
		beamtrail,
	};

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_particle_selection, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_PARTICLE_SELECTION_H_INCLUDED
