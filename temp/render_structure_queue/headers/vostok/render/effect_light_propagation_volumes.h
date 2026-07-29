////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED
#define RENDER_EFFECT_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_light_propagation_volumes : public effect_descriptor {
public:
	virtual	void	compile								( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_light_propagation_volumes	( ) { /* no source */ }
	virtual			~effect_light_propagation_volumes	( ) { /* no source */ }
}; // class effect_light_propagation_volumes

STATIC_SIZE_ASSERT(effect_light_propagation_volumes, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED
