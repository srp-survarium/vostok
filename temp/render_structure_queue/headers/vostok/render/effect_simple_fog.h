////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_SIMPLE_FOG_H_INCLUDED
#define RENDER_EFFECT_SIMPLE_FOG_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_simple_fog : public effect_descriptor {
public:
	virtual	void	compile				( effect_compiler& compiler, custom_config_value const& __formal ) override;

	inline			effect_simple_fog	( ) { /* no source */ }
	virtual			~effect_simple_fog	( ) { /* no source */ }
}; // class effect_simple_fog

STATIC_SIZE_ASSERT(effect_simple_fog, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_SIMPLE_FOG_H_INCLUDED
