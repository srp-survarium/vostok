////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_PLANE_SPOT_LIGHT_EFFECT_H_INCLUDED
#define RENDER_PLANE_SPOT_LIGHT_EFFECT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class plane_spot_light_effect< 1 > : public effect_descriptor {
	virtual	void	compile						( effect_compiler& arg_0, custom_config_value const& arg_1 ) override { /* no source */ }

public:
	inline			plane_spot_light_effect<1>	( ) { /* no source */ }
	virtual			~plane_spot_light_effect<1>	( ) { /* no source */ }
}; // class plane_spot_light_effect< 1 >

STATIC_SIZE_ASSERT(plane_spot_light_effect< 1 >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_PLANE_SPOT_LIGHT_EFFECT_H_INCLUDED
