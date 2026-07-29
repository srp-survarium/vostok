////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_REFLECTION_MASK_H_INCLUDED
#define RENDER_EFFECT_REFLECTION_MASK_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_reflection_mask : public effect_descriptor {
public:
	virtual	void	compile					( effect_compiler& compiler, custom_config_value const& __formal ) override;

	inline			effect_reflection_mask	( ) { /* no source */ }
	virtual			~effect_reflection_mask	( ) { /* no source */ }
}; // class effect_reflection_mask

STATIC_SIZE_ASSERT(effect_reflection_mask, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_REFLECTION_MASK_H_INCLUDED
