////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_UNTYPED_BUFFER_H_INCLUDED
#define RENDER_UNTYPED_BUFFER_H_INCLUDED

/* INCLUDES */
struct ID3D11Buffer;
class vostok::render::resource_intrusive_base;
class vostok::render::enum_buffer_type;

namespace vostok {
namespace render {

enum vostok::render::enum_buffer_type
{
};

class untyped_buffer : public resource_intrusive_base {
public:
								untyped_buffer	(
									u32					size,
									pcvoid				data,
									enum_buffer_type	type,
									bool				is_dynamic,
									bool				staging
								);
								~untyped_buffer	( );

			void				destroy_impl	( ) const;

	inline	void*				map				( D3D11_MAP arg_0 ) { /* no source */ }

	inline	void				unmap			( ) { /* no source */ }

	inline	ID3D11Buffer*		hardware_buffer	( ) const { /* no source */ }

	inline	u32					type			( ) const { /* no source */ }

	inline	u32					size			( ) const { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
private:
	/* 0x0004 */	ID3D11Buffer*		m_hardware_buffer;
	/* 0x0008 */	u32					m_size;
	/* 0x000c */	enum_buffer_type	m_type;
}; // class untyped_buffer

STATIC_SIZE_ASSERT(untyped_buffer, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_UNTYPED_BUFFER_H_INCLUDED
