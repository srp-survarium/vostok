////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_STATE_H_INCLUDED
#define RENDER_RES_STATE_H_INCLUDED

/* INCLUDES */
struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
class vostok::render::resource_intrusive_base;

namespace vostok {
namespace render {

class res_state : public resource_intrusive_base {
protected:
					res_state		(
						ID3D11RasterizerState*		rasterizer_state,
						ID3D11DepthStencilState*	depth_stencil_state,
						ID3D11BlendState*			blend_state,
						u32							stencil_ref
					);
public:
					~res_state		( );

			void	destroy_impl	( ) const;

			void	apply			( ) const;

	inline	u32		stencil_ref		( ) { /* no source */ }

	inline	bool	is_registered	( ) const { /* no source */ }
	inline	void	mark_registered	( ) { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
private:
	/* 0x0004 */	ID3D11RasterizerState*		m_rasterizer_state;
	/* 0x0008 */	ID3D11DepthStencilState*	m_depth_stencil_state;
	/* 0x000c */	ID3D11BlendState*			m_blend_state;
	/* 0x0010 */	u32							m_stencil_ref;
	/* 0x0014 */	bool						m_is_registered;
}; // class res_state

STATIC_SIZE_ASSERT(res_state, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_STATE_H_INCLUDED
