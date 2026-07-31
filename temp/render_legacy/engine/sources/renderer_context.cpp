////////////////////////////////////////////////////////////////////////////
//	Created		: 19.01.2010
//	Author		: Armen Abroyan
//	Copyright ( C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER (2026-07-31): all renderer_context method bodies ported to
// sources/vostok/render/engine/sources/renderer_context.cpp. What remains here
// are the console-command globals backing the legacy inline getters
// get_lighting_material_strtegy()/get_shadow_map_z_bias() - the canonical
// header (ground truth) hardcodes `return 0;` / `return 0.0f;` for both, so
// these were NOT adopted. Kept as reference for the stage_sun/stage_lights
// harvest (they branch on the strategy value).

u32 s_lighting_material_strategy = 0;
static vostok::console_commands::cc_u32 s_cc_lighting_material_strategy ("lighting_material_strategy", s_lighting_material_strategy, 0, 4, true, vostok::console_commands::command_type_engine_internal);

float s_shadow_map_z_bias = 0.0001f;
static vostok::console_commands::cc_float s_cc_shadow_map_z_bias("shadow_map_z_bias", s_shadow_map_z_bias, 0, 4, true, vostok::console_commands::command_type_engine_internal);
