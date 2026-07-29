////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_EFFECT_H_INCLUDED
#define RENDER_RES_EFFECT_H_INCLUDED

/* INCLUDES */
class vostok::render::vector<vostok::render::texture_named_instance>;
class vostok::resources::unmanaged_resource;
typedef vostok::render::vector<vostok::intrusive_ptr<vostok::render::res_shader_technique,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >
	vostok::render::vector<vostok::render::res_shader_technique_ptr >;
class vostok::render::res_shader_technique;

/* FORWARD REFS */
class vostok::render::res_declaration;
class vostok::render::res_texture;

namespace vostok {
namespace render {

class res_effect : public resources::unmanaged_resource {
	virtual									~res_effect						( );

	inline	void							destroy_impl					( ) const { /* no source */ }

public:
			void							push_texture_unique				( res_texture* in_texture, pcstr path );

	inline									res_effect						( ) { /* no source */ }

	inline	void							get_max_used_texture_dimension	( u32& arg_0, u32& arg_1 ) { /* no source */ }
	inline	u32								get_max_pixel_shader_instruction_count( ) const { /* no source */ }

	inline	void							mark_registered					( ) { /* no source */ }
	inline	bool							is_registered					( ) const { /* no source */ }

	inline	void							init_layouts					( res_declaration const& arg_0 ) { /* no source */ }

	inline	bool							equal							( res_effect const& arg_0 ) const { /* no source */ }
	inline	bool							equal							( res_effect const* arg_0 ) const { /* no source */ }

	inline	bool							apply							( u32 technique_id, u32 pass_id ) { /* no source */ }

	inline	bool							select_technique				( u32 arg_0 ) { /* no source */ }

			bool							apply_pass						( u32 id );

	inline	res_shader_technique const*		get_technique					( u32 arg_0 ) const { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	vector< texture_named_instance >		m_used_textures;
private:
	/* 0x0114 */	u32										m_cur_technique;
public:
	/* 0x0118 */	vector< res_shader_technique_ptr >		m_techniques;
private:
	/* 0x0124 */	bool									m_registered;
}; // class res_effect

STATIC_SIZE_ASSERT(res_effect, 0x128);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_EFFECT_H_INCLUDED
