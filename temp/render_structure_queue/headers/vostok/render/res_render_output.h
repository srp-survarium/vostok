////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_RENDER_OUTPUT_H_INCLUDED
#define RENDER_RES_RENDER_OUTPUT_H_INCLUDED

/* INCLUDES */
struct DXGI_SWAP_CHAIN_DESC;
struct HWND__;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct IDXGISwapChain;
class vostok::fixed_string<128>;
class vostok::render::resource_intrusive_base;
class vostok::math::uint2;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

namespace vostok {
namespace render {

class res_render_output : public resource_intrusive_base {
public:
								res_render_output			( HWND__* window, bool windowed );
								~res_render_output			( );

			void				destroy_impl				( ) const;

			void				present						( );

			void				resize						( bool force_resize );
			void				resize						(
									bool			windowed,
									const u32		size_x,
									const u32		size_y,
									bool			force_resize
								);

			void				set_size					(
									const u32		in_width,
									const u32		in_height,
									const bool		in_fullscreen,
									bool			force_resize
								);

	inline	math::uint2			size						( ) const { /* no source */ }

	inline	u32					width						( ) const { /* no source */ }

	inline	u32					height						( ) const { /* no source */ }

	inline	HWND__*				window						( ) const { /* no source */ }

	inline	bool				equal						( HWND__* arg_0 ) const { /* no source */ }

	inline	bool				valid_present				( ) const { /* no source */ }

			void				goto_fullscreen				( );

	inline	bool				is_registered				( ) const { /* no source */ }
	inline	void				mark_registered				( ) { /* no source */ }

private:
			void				initialize_swap_chain		( IDXGISwapChain* swap_chain );

			void				select_resolution			(
									u32&		width,
									u32&		height,
									bool		windowed,
									HWND__*		window
								) const;
			DXGI_RATIONAL		select_refresh				( u32 width, u32 height, DXGI_FORMAT fmt ) const;

			void				update_targets				( );
			void				update_depth_stencil_buffer	( );
			void				update_window_properties	( );

			u32					select_presentation_interval( );

	/* 0x0000 */	/* resource_intrusive_base */
public:
	/* 0x0004 */	fixed_string< 128 >			m_depth_rexture_name;
	/* 0x0090 */	DXGI_SWAP_CHAIN_DESC		m_swap_chain_desc;
	/* 0x00cc */	IDXGISwapChain*				m_swap_chain;
	/* 0x00d0 */	ID3D11RenderTargetView*		m_base_rt;
	/* 0x00d4 */	ID3D11DepthStencilView*		m_base_zb;
	/* 0x00d8 */	res_texture_ptr				m_texture_zb;
	/* 0x00dc */	HWND__*						m_window;
	/* 0x00e0 */	u32							m_present_sync_mode;
	/* 0x00e4 */	bool						m_valid_previous_present;
	/* 0x00e5 */	bool						m_windowed;
	/* 0x00e6 */	bool						m_is_registered;
}; // class res_render_output

STATIC_SIZE_ASSERT(res_render_output, 0xE8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_RENDER_OUTPUT_H_INCLUDED
