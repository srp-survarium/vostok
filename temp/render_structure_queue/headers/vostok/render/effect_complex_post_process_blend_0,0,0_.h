////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_COMPLEX_POST_PROCESS_BLEND_H_INCLUDED
#define RENDER_EFFECT_COMPLEX_POST_PROCESS_BLEND_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_complex_post_process_blend< 0, 0, 0 > : public effect_descriptor {
public:
	virtual	void	compile	( effect_compiler& compiler, custom_config_value const& custom_config ) override { /* no source */ }

	inline			effect_complex_post_process_blend<0,0,0>( ) { /* no source */ }
	virtual			~effect_complex_post_process_blend<0,0,0>( ) { /* no source */ }
}; // class effect_complex_post_process_blend< 0, 0, 0 >

STATIC_SIZE_ASSERT(effect_complex_post_process_blend< 0, 0, 0 >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_COMPLEX_POST_PROCESS_BLEND_H_INCLUDED
