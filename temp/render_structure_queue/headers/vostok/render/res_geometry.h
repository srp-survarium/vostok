////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_GEOMETRY_H_INCLUDED
#define RENDER_RES_GEOMETRY_H_INCLUDED

/* INCLUDES */
class vostok::render::resource_intrusive_base;
class vostok::render::untyped_buffer const ;
typedef vostok::intrusive_ptr<vostok::render::res_declaration,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_declaration_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::res_declaration;
class vostok::render::untyped_buffer;

namespace vostok {
namespace render {

class res_geometry : public resource_intrusive_base {
	inline									res_geometry	(
												untyped_buffer&		vb,
												untyped_buffer&		ib,
												res_declaration*	dcl,
												u32					stride
											) { /* no source */ }
											~res_geometry	( );

			void							destroy_impl	( ) const;

public:
	inline	bool							equal			( res_geometry const& arg_0 ) const { /* no source */ }

			s32								compare			( res_geometry const& other ) const;

			void							apply			( );

	inline	bool							is_registered	( ) const { /* no source */ }
	inline	void							mark_registered	( ) { /* no source */ }

	inline	u32								get_stride		( ) { /* no source */ }

	inline	untyped_buffer const _ptr		vertex_buffer	( ) { /* no source */ }
	inline	untyped_buffer const _ptr		intex_buffer	( ) { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
	/* 0x0004 */	untyped_buffer_ptr		m_vb;
	/* 0x0008 */	untyped_buffer_ptr		m_ib;
	/* 0x000c */	u32						m_vb_stride;
	/* 0x0010 */	res_declaration_ptr		m_dcl;
private:
	/* 0x0014 */	bool					m_is_registered;
}; // class res_geometry

STATIC_SIZE_ASSERT(res_geometry, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_GEOMETRY_H_INCLUDED
