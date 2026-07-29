////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_CLOUDS_H_INCLUDED
#define RENDER_EFFECT_CLOUDS_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_clouds : public effect_descriptor {
public:
	virtual	void	compile			( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_clouds	( ) { /* no source */ }
	virtual			~effect_clouds	( ) { /* no source */ }
}; // class effect_clouds

STATIC_SIZE_ASSERT(effect_clouds, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_CLOUDS_H_INCLUDED
