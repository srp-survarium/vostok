////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED
#define RENDER_EFFECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_sky_ambient_occlusion : public effect_descriptor {
public:
	virtual	void	compile							( effect_compiler& compiler, custom_config_value const& config ) override { /* no source */ }

	inline			effect_sky_ambient_occlusion	( ) { /* no source */ }
	virtual			~effect_sky_ambient_occlusion	( ) { /* no source */ }
}; // class effect_sky_ambient_occlusion

STATIC_SIZE_ASSERT(effect_sky_ambient_occlusion, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED
