////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_HIZ_OCCLUSION_H_INCLUDED
#define RENDER_EFFECT_HIZ_OCCLUSION_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_hiz_occlusion : public effect_descriptor {
public:
	virtual	void	compile					( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_hiz_occlusion	( ) { /* no source */ }
	virtual			~effect_hiz_occlusion	( ) { /* no source */ }
}; // class effect_hiz_occlusion

STATIC_SIZE_ASSERT(effect_hiz_occlusion, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_HIZ_OCCLUSION_H_INCLUDED
