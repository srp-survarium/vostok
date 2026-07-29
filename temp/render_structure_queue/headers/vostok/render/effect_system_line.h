////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_SYSTEM_LINE_H_INCLUDED
#define RENDER_EFFECT_SYSTEM_LINE_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_system_line : public effect_descriptor {
public:
	inline			effect_system_line	( ) { /* no source */ }
	virtual			~effect_system_line	( ) { /* no source */ }

	virtual	void	compile				( effect_compiler& c, custom_config_value const& config ) override;
}; // class effect_system_line

STATIC_SIZE_ASSERT(effect_system_line, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_SYSTEM_LINE_H_INCLUDED
