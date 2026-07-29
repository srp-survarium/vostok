////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_DECLARATION_H_INCLUDED
#define RENDER_RES_DECLARATION_H_INCLUDED

/* INCLUDES */
class vostok::render::resource_intrusive_base;
class vostok::render::vector<D3D11_INPUT_ELEMENT_DESC>;
class vostok::render::vector<vostok::render::signature_layout_pair>;

/* FORWARD REFS */
class vostok::render::res_input_layout;
class vostok::render::res_signature;

namespace vostok {
namespace render {

class res_declaration : public resource_intrusive_base , public boost::noncopyable {
public:
									res_declaration	( D3D11_INPUT_ELEMENT_DESC const* decl, u32 count );
									~res_declaration( );

			void					destroy_impl	( ) const;

	inline	bool					equal			( D3D11_INPUT_ELEMENT_DESC const* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	res_input_layout*		get				( res_signature const* signature ) { /* no source */ }

	inline	bool					is_registered	( ) const { /* no source */ }
	inline	void					mark_registered	( ) { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	vector< signature_layout_pair >		vs_to_layout;
	/* 0x0010 */	const vector< D3D11_INPUT_ELEMENT_DESC >	dcl_code;
	/* 0x001c */	bool								m_is_registered;
}; // class res_declaration

STATIC_SIZE_ASSERT(res_declaration, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_DECLARATION_H_INCLUDED
