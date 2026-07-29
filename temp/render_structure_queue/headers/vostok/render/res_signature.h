////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_SIGNATURE_H_INCLUDED
#define RENDER_RES_SIGNATURE_H_INCLUDED

/* INCLUDES */
struct ID3D10Blob;
class vostok::render::resource_intrusive_base;

namespace vostok {
namespace render {

class res_signature : public resource_intrusive_base {
	inline	explicit		res_signature	( ID3D10Blob* arg_0 ) { /* no source */ }
							~res_signature	( );

			void			destroy_impl	( ) const;

public:
	inline	ID3D10Blob*		hw_signature	( ) const { /* no source */ }

	inline	bool			equal			( ID3D10Blob const* arg_0 ) { /* no source */ }

	inline	bool			is_registered	( ) const { /* no source */ }
	inline	void			mark_registered	( ) { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
private:
	/* 0x0004 */	ID3D10Blob*		m_signature;
	/* 0x0008 */	bool			m_is_registered;
}; // class res_signature

STATIC_SIZE_ASSERT(res_signature, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_SIGNATURE_H_INCLUDED
