#ifndef VOSTOK_RENDER_ENGINE_BOX_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_BOX_GEOMETRY_H_INCLUDED

#include <vostok/detail_noncopyable.h>
#include <vostok/math_float4.h>
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/untyped_buffer.h>

namespace vostok {
namespace render {

struct box_geometry : public core::noncopyable {
	struct vertex_type {
		vertex_type( ) { }

		float4 position;
	};

	box_geometry( );
	~box_geometry( ) { }

	void draw( );

private:
	res_declaration_ptr	m_vertext_declaration;
	untyped_buffer_ptr	m_vertex_buffer;
	untyped_buffer_ptr	m_index_buffer;
	u32 const			m_stride;
};

STATIC_SIZE_ASSERT( box_geometry::vertex_type, 0x10 );
STATIC_SIZE_ASSERT( box_geometry, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_BOX_GEOMETRY_H_INCLUDED
