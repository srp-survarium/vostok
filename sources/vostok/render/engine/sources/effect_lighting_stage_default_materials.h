// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_EFFECT_LIGHTING_STAGE_DEFAULT_MATERIALS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_LIGHTING_STAGE_DEFAULT_MATERIALS_H_INCLUDED
#include "effect_material_base.h"

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_lighting_stage_default_materials : public effect_material_base {
public:
	enum {
		brdf_lighting,
		probe_lighting
	};

	virtual bool should_recompile_when_global_changes(
		vector< fs_new::virtual_path_string > const& changed_defines
	) const override;

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_lighting_stage_default_materials, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_LIGHTING_STAGE_DEFAULT_MATERIALS_H_INCLUDED
