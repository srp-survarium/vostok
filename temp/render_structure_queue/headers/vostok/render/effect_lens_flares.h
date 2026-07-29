////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_LENS_FLARES_H_INCLUDED
#define RENDER_EFFECT_LENS_FLARES_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_lens_flares : public effect_descriptor {
public:
	virtual	void	compile				( effect_compiler& compiler, custom_config_value const& custom_config ) override;

	inline			effect_lens_flares	( ) { /* no source */ }
	virtual			~effect_lens_flares	( ) { /* no source */ }
}; // class effect_lens_flares

STATIC_SIZE_ASSERT(effect_lens_flares, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_LENS_FLARES_H_INCLUDED
