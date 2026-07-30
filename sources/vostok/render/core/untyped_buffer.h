#ifndef VOSTOK_RENDER_CORE_UNTYPED_BUFFER_H_INCLUDED
#define VOSTOK_RENDER_CORE_UNTYPED_BUFFER_H_INCLUDED

#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/resource_intrusive_base.h>

namespace vostok {
namespace render {

enum enum_buffer_type {
	enum_buffer_type_vertex	= 0,
	enum_buffer_type_index	= 1,
};

class untyped_buffer : public resource_intrusive_base {
public:
	untyped_buffer(
		u32					size,
		pcvoid				data,
		enum_buffer_type	type,
		bool				is_dynamic,
		bool				staging
	);

	~untyped_buffer( );

	void destroy_impl( ) const;

	void* map( D3D11_MAP ) { return 0; }
	void unmap( ) { }
	ID3D11Buffer* hardware_buffer( ) const { return m_hardware_buffer; }
	u32 type( ) const { return m_type; }
	u32 size( ) const { return m_size; }

private:
	ID3D11Buffer*		m_hardware_buffer;
	u32					m_size;
	enum_buffer_type	m_type;
};

STATIC_SIZE_ASSERT( untyped_buffer, 0x10 );

typedef intrusive_ptr<
	untyped_buffer,
	resource_intrusive_base,
	threading::single_threading_policy
> untyped_buffer_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_UNTYPED_BUFFER_H_INCLUDED
