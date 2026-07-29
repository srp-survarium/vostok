////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CAPSULE_LIGHT_EFFECT_H_INCLUDED
#define RENDER_CAPSULE_LIGHT_EFFECT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class capsule_light_effect : public effect_descriptor {
	virtual	void	compile					( effect_compiler& compiler, custom_config_value const& config ) override { /* no source */ }

public:
	inline			capsule_light_effect	( ) { /* no source */ }
	virtual			~capsule_light_effect	( ) { /* no source */ }
}; // class capsule_light_effect

STATIC_SIZE_ASSERT(capsule_light_effect, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CAPSULE_LIGHT_EFFECT_H_INCLUDED
