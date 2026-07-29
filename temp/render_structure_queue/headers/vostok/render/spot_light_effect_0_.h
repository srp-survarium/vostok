////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPOT_LIGHT_EFFECT_H_INCLUDED
#define RENDER_SPOT_LIGHT_EFFECT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class spot_light_effect< 0 > : public effect_descriptor {
	virtual	void	compile					( effect_compiler& compiler, custom_config_value const& config ) override { /* no source */ }

public:
	inline			spot_light_effect<0>	( ) { /* no source */ }
	virtual			~spot_light_effect<0>	( ) { /* no source */ }
}; // class spot_light_effect< 0 >

STATIC_SIZE_ASSERT(spot_light_effect< 0 >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPOT_LIGHT_EFFECT_H_INCLUDED
