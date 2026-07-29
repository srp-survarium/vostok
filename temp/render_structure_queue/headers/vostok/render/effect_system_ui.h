////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_SYSTEM_UI_H_INCLUDED
#define RENDER_EFFECT_SYSTEM_UI_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_system_ui : public effect_descriptor {
public:
	inline			effect_system_ui	( ) { /* no source */ }
	virtual			~effect_system_ui	( ) { /* no source */ }

	virtual	void	compile				( effect_compiler& c, custom_config_value const& config ) override;
}; // class effect_system_ui

STATIC_SIZE_ASSERT(effect_system_ui, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_SYSTEM_UI_H_INCLUDED
