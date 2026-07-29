////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_COPY_DEPTH_RT_H_INCLUDED
#define RENDER_EFFECT_COPY_DEPTH_RT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_copy_depth_rt : public effect_descriptor {
public:
	virtual	void	compile					( effect_compiler& compiler, custom_config_value const& __formal ) override;

	inline			effect_copy_depth_rt	( ) { /* no source */ }
	virtual			~effect_copy_depth_rt	( ) { /* no source */ }
}; // class effect_copy_depth_rt

STATIC_SIZE_ASSERT(effect_copy_depth_rt, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_COPY_DEPTH_RT_H_INCLUDED
