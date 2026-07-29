////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_ENVIRONMENT_PROBE_LIGHTING_H_INCLUDED
#define RENDER_EFFECT_ENVIRONMENT_PROBE_LIGHTING_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_environment_probe_lighting< 0, 1, 0 > : public effect_descriptor {
public:
	virtual	void	compile	( effect_compiler& arg_0, custom_config_value const& arg_1 ) override { /* no source */ }

	inline			effect_environment_probe_lighting<0,1,0>( ) { /* no source */ }
	virtual			~effect_environment_probe_lighting<0,1,0>( ) { /* no source */ }
}; // class effect_environment_probe_lighting< 0, 1, 0 >

STATIC_SIZE_ASSERT(effect_environment_probe_lighting< 0, 1, 0 >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_ENVIRONMENT_PROBE_LIGHTING_H_INCLUDED
