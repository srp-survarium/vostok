////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_SHADER_TECHNIQUE_H_INCLUDED
#define RENDER_RES_SHADER_TECHNIQUE_H_INCLUDED

/* INCLUDES */
class vostok::render::resource_intrusive_base;
struct vostok::render::res_shader_technique::sflags;
typedef vostok::render::vector<vostok::intrusive_ptr<vostok::render::res_pass,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >
	vostok::render::vector<vostok::render::res_pass_ptr >;
class vostok::render::res_pass;

/* FORWARD REFS */
class vostok::render::res_declaration;

namespace vostok {
namespace render {

class res_shader_technique : public resource_intrusive_base {
public:
	inline						res_shader_technique	( ) { /* no source */ }
								~res_shader_technique	( );

			void				destroy_impl			( ) const;

	inline	void				mark_registered			( ) { /* no source */ }
	inline	bool				is_registered			( ) const { /* no source */ }

	inline	void				init_layouts			( res_declaration const& arg_0 ) { /* no source */ }

	inline	bool				equal					( res_shader_technique const& arg_0 ) const { /* no source */ }
	inline	bool				equal					( res_shader_technique const* arg_0 ) const { /* no source */ }

	inline	res_pass const*		get_pass				( u32 arg_0 ) const { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
	/* 0x0004 */	res_shader_technique::sflags	m_flags;
	/* 0x0008 */	vector< res_pass_ptr >			m_passes;
	/* 0x0014 */	bool							m_registered;
}; // class res_shader_technique

STATIC_SIZE_ASSERT(res_shader_technique, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_SHADER_TECHNIQUE_H_INCLUDED
