////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_SCREEN_IMAGE_H_INCLUDED
#define RENDER_STAGE_SCREEN_IMAGE_H_INCLUDED

/* INCLUDES */
class vostok::render::res_texture_list;
class vostok::render::stage;
class vostok::render::res_texture;
typedef vostok::intrusive_ptr<vostok::render::res_declaration,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_declaration_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_declaration;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_screen_image : public stage {
public:
	inline			stage_screen_image	( renderer* arg_0, renderer_context* arg_1 ) { /* no source */ }
	virtual			~stage_screen_image	( ) { /* no source */ }

			void	execute				( res_texture_ptr t_image );

private:
			bool	is_effects_ready	( ) const;

	/* 0x0000 */	/* stage */
	/* 0x0010 */	res_effect_ptr			m_present_effect;
	/* 0x0014 */	res_declaration_ptr		m_decl_ptr;
	/* 0x0018 */	res_texture_list		m_textures;
}; // class stage_screen_image

STATIC_SIZE_ASSERT(stage_screen_image, 0x2C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_SCREEN_IMAGE_H_INCLUDED
