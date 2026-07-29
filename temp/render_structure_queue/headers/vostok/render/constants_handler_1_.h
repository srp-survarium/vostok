////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CONSTANTS_HANDLER_H_INCLUDED
#define RENDER_CONSTANTS_HANDLER_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::shader_constant_table const ,vostok::render::resource_intrusive_base const ,vostok::threading::single_threading_policy>
	vostok::render::shader_constant_table const _ptr;
class vostok::render::shader_constant_table const ;

/* FORWARD REFS */
class vostok::render::shader_constant_table;

namespace vostok {
namespace render {

class constants_handler< 1 > {
	inline			constants_handler<1>		( ) { /* no source */ }

	inline	void	assign						( shader_constant_table const* arg_0 ) { /* no source */ }

	inline	void	gather_data					( ) { /* no source */ }

	inline	void	update_buffers				( ) { /* no source */ }

			void	apply						( );

	inline	void	fill_changes_buffer			( ID3D11Buffer** arg_0, u32& arg_1 ) { /* no source */ }

	inline	void	check_for_unset_constants	( ) { /* no source */ }

public:
	inline			~constants_handler<1>		( ) { /* no source */ }

private:
	/* 0x0000 */	u32									m_diff_range_start;
	/* 0x0004 */	u32									m_diff_range_end;
	/* 0x0008 */	shader_constant_table const _ptr	m_current;
}; // class constants_handler< 1 >

STATIC_SIZE_ASSERT(constants_handler< 1 >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CONSTANTS_HANDLER_H_INCLUDED
