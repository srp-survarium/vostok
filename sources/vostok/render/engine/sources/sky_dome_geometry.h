#ifndef VOSTOK_RENDER_ENGINE_SKY_DOME_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SKY_DOME_GEOMETRY_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/math_float2.h>
#include <vostok/math_float4.h>
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/untyped_buffer.h>

namespace vostok {
namespace render {

struct sky_dome_geometry : public boost::noncopyable {
	struct vertex_type {
		float4	position;
		float2	uv;
	};

	sky_dome_geometry( );
	void draw( );

	res_declaration_ptr	m_vertext_declaration;
	untyped_buffer_ptr	m_vertex_buffer;
	untyped_buffer_ptr	m_index_buffer;
	u32 const			m_stride;
	u32					m_num_indices;
};

STATIC_SIZE_ASSERT( sky_dome_geometry::vertex_type, 0x18 );
STATIC_SIZE_ASSERT( sky_dome_geometry, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SKY_DOME_GEOMETRY_H_INCLUDED
