////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_DEBUG_ENVIRONMENT_PROBE_PREVIEW_H_INCLUDED
#define RENDER_EFFECT_DEBUG_ENVIRONMENT_PROBE_PREVIEW_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_debug_environment_probe_preview : public effect_descriptor {
public:
	virtual	void	compile	( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_debug_environment_probe_preview( ) { /* no source */ }
	virtual			~effect_debug_environment_probe_preview( ) { /* no source */ }
}; // class effect_debug_environment_probe_preview

STATIC_SIZE_ASSERT(effect_debug_environment_probe_preview, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_DEBUG_ENVIRONMENT_PROBE_PREVIEW_H_INCLUDED
