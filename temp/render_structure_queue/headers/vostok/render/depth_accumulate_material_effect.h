////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DEPTH_ACCUMULATE_MATERIAL_EFFECT_H_INCLUDED
#define RENDER_DEPTH_ACCUMULATE_MATERIAL_EFFECT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class depth_accumulate_material_effect : public effect_material_base {
	virtual	void	compile								( effect_compiler& compiler, custom_config_value const& config ) override;

public:
	inline			depth_accumulate_material_effect	( ) { /* no source */ }
	virtual			~depth_accumulate_material_effect	( ) { /* no source */ }
}; // class depth_accumulate_material_effect

STATIC_SIZE_ASSERT(depth_accumulate_material_effect, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DEPTH_ACCUMULATE_MATERIAL_EFFECT_H_INCLUDED
