////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_EXPONENTIAL_VOLUME_FOG_H_INCLUDED
#define RENDER_EFFECT_EXPONENTIAL_VOLUME_FOG_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_exponential_volume_fog : public effect_material_base {
public:
	virtual	void	compile							( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_exponential_volume_fog	( ) { /* no source */ }
	virtual			~effect_exponential_volume_fog	( ) { /* no source */ }
}; // class effect_exponential_volume_fog

STATIC_SIZE_ASSERT(effect_exponential_volume_fog, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_EXPONENTIAL_VOLUME_FOG_H_INCLUDED
