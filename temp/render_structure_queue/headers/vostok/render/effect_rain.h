////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_RAIN_H_INCLUDED
#define RENDER_EFFECT_RAIN_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_rain : public effect_descriptor {
public:
	virtual	void	compile		( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_rain	( ) { /* no source */ }
	virtual			~effect_rain( ) { /* no source */ }
}; // class effect_rain

STATIC_SIZE_ASSERT(effect_rain, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_RAIN_H_INCLUDED
