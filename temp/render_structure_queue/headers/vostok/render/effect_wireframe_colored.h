////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_WIREFRAME_COLORED_H_INCLUDED
#define RENDER_EFFECT_WIREFRAME_COLORED_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_wireframe_colored : public effect_descriptor {
public:
	inline			effect_wireframe_colored	( ) { /* no source */ }
	virtual			~effect_wireframe_colored	( ) { /* no source */ }

	virtual	void	compile						( effect_compiler& compiler, custom_config_value const& config ) override;
}; // class effect_wireframe_colored

STATIC_SIZE_ASSERT(effect_wireframe_colored, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_WIREFRAME_COLORED_H_INCLUDED
