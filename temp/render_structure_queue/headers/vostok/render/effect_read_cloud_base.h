////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_READ_CLOUD_BASE_H_INCLUDED
#define RENDER_EFFECT_READ_CLOUD_BASE_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_read_cloud_base : public effect_descriptor {
public:
	virtual	void	compile					( effect_compiler& compiler, custom_config_value const& config ) override;

	inline			effect_read_cloud_base	( ) { /* no source */ }
	virtual			~effect_read_cloud_base	( ) { /* no source */ }
}; // class effect_read_cloud_base

STATIC_SIZE_ASSERT(effect_read_cloud_base, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_READ_CLOUD_BASE_H_INCLUDED
