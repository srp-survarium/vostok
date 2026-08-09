#ifndef VOSTOK_RENDER_ENGINE_STAGE_SCREEN_IMAGE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_SCREEN_IMAGE_H_INCLUDED

#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_texture_list.h>

#include "res_texture.h"
#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_screen_image : public stage {
public:
	stage_screen_image( renderer* in_renderer, renderer_context* context );

	virtual ~stage_screen_image( ) { }

	void execute( res_texture_ptr image );

	bool is_effects_ready( ) const;

private:
	res_effect_ptr		m_present_effect;
	res_declaration_ptr	m_decl_ptr;
	res_texture_list	m_textures;
};

STATIC_SIZE_ASSERT( stage_screen_image, 0x2C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_SCREEN_IMAGE_H_INCLUDED
