////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_COPY_IMAGE_H_INCLUDED
#define RENDER_EFFECT_COPY_IMAGE_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_copy_image : public effect_descriptor {
public:
	inline			effect_copy_image	( ) { /* no source */ }

	virtual	void	compile				( effect_compiler& compiler, custom_config_value const& custom_config ) override;

	virtual			~effect_copy_image	( ) { /* no source */ }
}; // class effect_copy_image

STATIC_SIZE_ASSERT(effect_copy_image, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_COPY_IMAGE_H_INCLUDED
