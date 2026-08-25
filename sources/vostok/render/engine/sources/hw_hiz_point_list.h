#ifndef VOSTOK_RENDER_ENGINE_HW_HIZ_POINT_LIST_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_HW_HIZ_POINT_LIST_H_INCLUDED

#include <vostok/math_float4.h>
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/untyped_buffer.h>

namespace vostok {
namespace render {

struct hw_hiz_point_list {
	hw_hiz_point_list( );

	void set_points( float4 const* in_data, u32 const culling_results_buffer_width );
	void initialize( u32 const num_points );
	void render( u32 const num_points );
	~hw_hiz_point_list( ) { }

private:
	res_declaration_ptr	m_declaration;
	untyped_buffer_ptr	m_vertex_buffer;
	u32					m_num_points;
};

STATIC_SIZE_ASSERT( hw_hiz_point_list, 0xC );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_HW_HIZ_POINT_LIST_H_INCLUDED
