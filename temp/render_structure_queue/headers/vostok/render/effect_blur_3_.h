////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_BLUR_H_INCLUDED
#define RENDER_EFFECT_BLUR_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_blur< 3 > : public effect_descriptor {
public:
	inline			effect_blur<3>	( ) { /* no source */ }

	virtual	void	compile			( effect_compiler& compiler, custom_config_value const& custom_config ) override { /* no source */ }

	virtual			~effect_blur<3>	( ) { /* no source */ }
}; // class effect_blur< 3 >

STATIC_SIZE_ASSERT(effect_blur< 3 >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_BLUR_H_INCLUDED
