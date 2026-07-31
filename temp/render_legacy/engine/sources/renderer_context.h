////////////////////////////////////////////////////////////////////////////
//	Created		: 02.09.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER (2026-07-31): the renderer_context class itself is fully
// covered by the canonical sources/vostok/render/engine/sources/renderer_context.h
// (members/inlines verified identical modulo type renames ref_texture ->
// res_texture_ptr, ref_geometry -> res_geometry_ptr). Kept below is ONLY what
// the canonical tree did NOT adopt:
// - enum_lighting_material_strategy: still referenced by the un-harvested
//   legacy stage_sun.cpp (lms_stenciling & co); no new-tree definition exists.
// - the extern globals: canonical get_lighting_material_strtegy()/
//   get_shadow_map_z_bias() hardcode 0 / 0.0f instead.

extern u32 s_lighting_material_strategy;
extern float s_shadow_map_z_bias;

namespace vostok {
namespace render {

enum enum_lighting_material_strategy
{
	lms_stenciling,
	lms_without_masking,
	lms_material_branching_separate,
	lms_material_branching_united
};

} // namespace render
} // namespace vostok
