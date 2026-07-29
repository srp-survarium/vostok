////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_HW_HIZ_POINT_LIST_H_INCLUDED
#define RENDER_HW_HIZ_POINT_LIST_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::res_declaration,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_declaration_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::res_declaration;
class vostok::render::untyped_buffer;

namespace vostok {
namespace render {

struct hw_hiz_point_list {
					hw_hiz_point_list	( );

			void	set_points			( float4 const* in_data, const u32 culling_results_buffer_width );

			void	initialize			( const u32 num_points );

			void	render				( const u32 num_points );

	inline			~hw_hiz_point_list	( ) { /* no source */ }

private:
	/* 0x0000 */	res_declaration_ptr		m_declaration;
	/* 0x0004 */	untyped_buffer_ptr		m_vertex_buffer;
	/* 0x0008 */	u32						m_num_points;
}; // struct hw_hiz_point_list

STATIC_SIZE_ASSERT(hw_hiz_point_list, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_HW_HIZ_POINT_LIST_H_INCLUDED
