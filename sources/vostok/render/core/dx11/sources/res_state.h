#ifndef VOSTOK_RENDER_CORE_DX11_RES_STATE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_STATE_H_INCLUDED

#include <vostok/render/core/resource_intrusive_base.h>

struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;

namespace vostok {
namespace render {

class res_state : public resource_intrusive_base {
protected:
	res_state(
		ID3D11RasterizerState*		rasterizer_state,
		ID3D11DepthStencilState*	depth_stencil_state,
		ID3D11BlendState*			blend_state,
		u32							stencil_ref
	);

public:
	~res_state( );

	void destroy_impl( ) const;
	void apply( ) const;

	u32 stencil_ref( ) const
	{
		return m_stencil_ref;
	}

	bool is_registered( ) const
	{
		return m_is_registered;
	}

	void mark_registered( )
	{
		m_is_registered = true;
	}

private:
	ID3D11RasterizerState*		m_rasterizer_state;
	ID3D11DepthStencilState*	m_depth_stencil_state;
	ID3D11BlendState*			m_blend_state;
	u32							m_stencil_ref;
	bool						m_is_registered;
};

STATIC_SIZE_ASSERT( res_state, 0x18 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_STATE_H_INCLUDED
