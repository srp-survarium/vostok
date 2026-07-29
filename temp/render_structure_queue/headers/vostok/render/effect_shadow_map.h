////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_SHADOW_MAP_H_INCLUDED
#define RENDER_EFFECT_SHADOW_MAP_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_shadow_map : public effect_descriptor {
public:
	virtual	void	compile				( effect_compiler& compiler, custom_config_value const& __formal ) override;

	inline			effect_shadow_map	( ) { /* no source */ }
	virtual			~effect_shadow_map	( ) { /* no source */ }
}; // class effect_shadow_map

STATIC_SIZE_ASSERT(effect_shadow_map, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_SHADOW_MAP_H_INCLUDED
