////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_INPUT_LAYOUT_H_INCLUDED
#define RENDER_RES_INPUT_LAYOUT_H_INCLUDED

/* INCLUDES */
struct ID3D11InputLayout;
class vostok::render::res_declaration;
class vostok::render::resource_intrusive_base;
typedef vostok::intrusive_ptr<vostok::render::res_signature const ,vostok::render::res_signature const ,vostok::threading::single_threading_policy>
	vostok::render::res_signature const _ptr;
class vostok::render::res_signature const ;

/* FORWARD REFS */
class vostok::render::res_signature;

namespace vostok {
namespace render {

class res_input_layout : public resource_intrusive_base {
public:
									res_input_layout	( res_declaration const* decl, res_signature const* signature );
									~res_input_layout	( );

			void					destroy_impl		( ) const;

	inline	bool					equal				( res_declaration const* arg_0, res_signature const* arg_1 ) const { /* no source */ }

	inline	bool					is_registered		( ) const { /* no source */ }
	inline	void					mark_registered		( ) { /* no source */ }

	inline	ID3D11InputLayout*		hardware_layout		( ) { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
private:
	/* 0x0004 */	ID3D11InputLayout*			m_hw_input_layout;
	/* 0x0008 */	res_declaration const*		m_declaration;
	/* 0x000c */	res_signature const _ptr	m_signature;
	/* 0x0010 */	bool						m_is_registered;
}; // class res_input_layout

STATIC_SIZE_ASSERT(res_input_layout, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_INPUT_LAYOUT_H_INCLUDED
