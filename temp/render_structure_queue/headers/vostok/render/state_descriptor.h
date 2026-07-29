////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATE_DESCRIPTOR_H_INCLUDED
#define RENDER_STATE_DESCRIPTOR_H_INCLUDED

/* INCLUDES */
struct D3D11_BLEND_DESC;
struct D3D11_DEPTH_STENCIL_DESC;
struct D3D11_RASTERIZER_DESC;

namespace vostok {
namespace render {

class state_descriptor {
public:
					state_descriptor		( );

			void	reset					( );

			void	set_depth				( bool enable, bool write_enable, D3D11_COMPARISON_FUNC cmp_func );
			void	set_stencil				(
						s32		enable,
						u32		ref,
						u8		read_mask,
						u8		write_mask
					);
	inline	void	set_stencil				(
						s32							arg_0,
						u32							arg_1,
						u8							arg_2,
						u8							arg_3,
						D3D11_COMPARISON_FUNC		arg_4,
						D3D11_STENCIL_OP			arg_5,
						D3D11_STENCIL_OP			arg_6,
						D3D11_STENCIL_OP			arg_7
					) { /* no source */ }
			void	set_stencil_frontface	(
						D3D11_COMPARISON_FUNC		func,
						D3D11_STENCIL_OP			fail,
						D3D11_STENCIL_OP			pass,
						D3D11_STENCIL_OP			zfail
					);
			void	set_stencil_backface	(
						D3D11_COMPARISON_FUNC		func,
						D3D11_STENCIL_OP			fail,
						D3D11_STENCIL_OP			pass,
						D3D11_STENCIL_OP			zfail
					);
	inline	void	set_stencil_ref			( u32 arg_0 ) { /* no source */ }
			void	set_fill_mode			( D3D11_FILL_MODE fill_mode );
			void	set_alpha_blend			(
						s32					blend_enable,
						D3D11_BLEND			src_blend,
						D3D11_BLEND			dest_blend,
						D3D11_BLEND_OP		blend_op,
						D3D11_BLEND			src_alpha_blend,
						D3D11_BLEND			dest_alpha_blend,
						D3D11_BLEND_OP		blend_alpha_op
					);
	inline	void	set_alpha_to_coverage	( s32 arg_0 ) { /* no source */ }

			void	color_write_enable		( D3D11_COLOR_WRITE_ENABLE mode );

			void	set_cull_mode			( D3D11_CULL_MODE mode );

private:
	/* 0x0000 */	D3D11_RASTERIZER_DESC		m_rasterizer_desc;
	/* 0x0028 */	D3D11_DEPTH_STENCIL_DESC	m_depth_stencil_desc;
	/* 0x005c */	D3D11_BLEND_DESC			m_effect_desc;
	/* 0x0164 */	u32							m_stencil_ref;
	/* 0x0168 */	bool						m_rasterizer_desc_updated;
	/* 0x0169 */	bool						m_depth_stencil_desc_updated;
	/* 0x016a */	bool						m_effect_desc_updated;
}; // class state_descriptor

STATIC_SIZE_ASSERT(state_descriptor, 0x16C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATE_DESCRIPTOR_H_INCLUDED
