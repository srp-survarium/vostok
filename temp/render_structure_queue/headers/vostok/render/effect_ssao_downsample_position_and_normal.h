////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_SSAO_DOWNSAMPLE_POSITION_AND_NORMAL_H_INCLUDED
#define RENDER_EFFECT_SSAO_DOWNSAMPLE_POSITION_AND_NORMAL_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_ssao_downsample_position_and_normal : public effect_descriptor {
public:
	virtual	void	compile	( effect_compiler& compiler, custom_config_value const& custom_config ) override;

	inline			effect_ssao_downsample_position_and_normal( ) { /* no source */ }
	virtual			~effect_ssao_downsample_position_and_normal( ) { /* no source */ }
}; // class effect_ssao_downsample_position_and_normal

STATIC_SIZE_ASSERT(effect_ssao_downsample_position_and_normal, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_SSAO_DOWNSAMPLE_POSITION_AND_NORMAL_H_INCLUDED
