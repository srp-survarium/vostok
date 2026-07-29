////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPHERE_LIGHT_EFFECT_H_INCLUDED
#define RENDER_SPHERE_LIGHT_EFFECT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class sphere_light_effect< 1 > : public effect_descriptor {
	virtual	void	compile					( effect_compiler& arg_0, custom_config_value const& arg_1 ) override { /* no source */ }

public:
	inline			sphere_light_effect<1>	( ) { /* no source */ }
	virtual			~sphere_light_effect<1>	( ) { /* no source */ }
}; // class sphere_light_effect< 1 >

STATIC_SIZE_ASSERT(sphere_light_effect< 1 >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPHERE_LIGHT_EFFECT_H_INCLUDED
