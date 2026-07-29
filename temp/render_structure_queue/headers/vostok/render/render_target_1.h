////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_TARGET_H_INCLUDED
#define RENDER_RENDER_TARGET_H_INCLUDED

/* INCLUDES */
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11Texture3D;
class vostok::render::resource_intrusive_base;
class vostok::shared_string;
class vostok::render::enum_rt_usage;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

namespace vostok {
namespace render {

enum DXGI_FORMAT
{
	DXGI_FORMAT_UNKNOWN						= 0x00,
	DXGI_FORMAT_R32G32B32A32_TYPELESS		= 0x01,
	DXGI_FORMAT_R32G32B32A32_FLOAT			= 0x02,
	DXGI_FORMAT_R32G32B32A32_UINT			= 0x03,
	DXGI_FORMAT_R32G32B32A32_SINT			= 0x04,
	DXGI_FORMAT_R32G32B32_TYPELESS			= 0x05,
	DXGI_FORMAT_R32G32B32_FLOAT				= 0x06,
	DXGI_FORMAT_R32G32B32_UINT				= 0x07,
	DXGI_FORMAT_R32G32B32_SINT				= 0x08,
	DXGI_FORMAT_R16G16B16A16_TYPELESS		= 0x09,
	DXGI_FORMAT_R16G16B16A16_FLOAT			= 0x0a,
	DXGI_FORMAT_R16G16B16A16_UNORM			= 0x0b,
	DXGI_FORMAT_R16G16B16A16_UINT			= 0x0c,
	DXGI_FORMAT_R16G16B16A16_SNORM			= 0x0d,
	DXGI_FORMAT_R16G16B16A16_SINT			= 0x0e,
	DXGI_FORMAT_R32G32_TYPELESS				= 0x0f,
	DXGI_FORMAT_R32G32_FLOAT				= 0x10,
	DXGI_FORMAT_R32G32_UINT					= 0x11,
	DXGI_FORMAT_R32G32_SINT					= 0x12,
	DXGI_FORMAT_R32G8X24_TYPELESS			= 0x13,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT		= 0x14,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS	= 0x15,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT		= 0x16,
	DXGI_FORMAT_R10G10B10A2_TYPELESS		= 0x17,
	DXGI_FORMAT_R10G10B10A2_UNORM			= 0x18,
	DXGI_FORMAT_R10G10B10A2_UINT			= 0x19,
	DXGI_FORMAT_R11G11B10_FLOAT				= 0x1a,
	DXGI_FORMAT_R8G8B8A8_TYPELESS			= 0x1b,
	DXGI_FORMAT_R8G8B8A8_UNORM				= 0x1c,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB			= 0x1d,
	DXGI_FORMAT_R8G8B8A8_UINT				= 0x1e,
	DXGI_FORMAT_R8G8B8A8_SNORM				= 0x1f,
	DXGI_FORMAT_R8G8B8A8_SINT				= 0x20,
	DXGI_FORMAT_R16G16_TYPELESS				= 0x21,
	DXGI_FORMAT_R16G16_FLOAT				= 0x22,
	DXGI_FORMAT_R16G16_UNORM				= 0x23,
	DXGI_FORMAT_R16G16_UINT					= 0x24,
	DXGI_FORMAT_R16G16_SNORM				= 0x25,
	DXGI_FORMAT_R16G16_SINT					= 0x26,
	DXGI_FORMAT_R32_TYPELESS				= 0x27,
	DXGI_FORMAT_D32_FLOAT					= 0x28,
	DXGI_FORMAT_R32_FLOAT					= 0x29,
	DXGI_FORMAT_R32_UINT					= 0x2a,
	DXGI_FORMAT_R32_SINT					= 0x2b,
	DXGI_FORMAT_R24G8_TYPELESS				= 0x2c,
	DXGI_FORMAT_D24_UNORM_S8_UINT			= 0x2d,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS		= 0x2e,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT		= 0x2f,
	DXGI_FORMAT_R8G8_TYPELESS				= 0x30,
	DXGI_FORMAT_R8G8_UNORM					= 0x31,
	DXGI_FORMAT_R8G8_UINT					= 0x32,
	DXGI_FORMAT_R8G8_SNORM					= 0x33,
	DXGI_FORMAT_R8G8_SINT					= 0x34,
	DXGI_FORMAT_R16_TYPELESS				= 0x35,
	DXGI_FORMAT_R16_FLOAT					= 0x36,
	DXGI_FORMAT_D16_UNORM					= 0x37,
	DXGI_FORMAT_R16_UNORM					= 0x38,
	DXGI_FORMAT_R16_UINT					= 0x39,
	DXGI_FORMAT_R16_SNORM					= 0x3a,
	DXGI_FORMAT_R16_SINT					= 0x3b,
	DXGI_FORMAT_R8_TYPELESS					= 0x3c,
	DXGI_FORMAT_R8_UNORM					= 0x3d,
	DXGI_FORMAT_R8_UINT						= 0x3e,
	DXGI_FORMAT_R8_SNORM					= 0x3f,
	DXGI_FORMAT_R8_SINT						= 0x40,
	DXGI_FORMAT_A8_UNORM					= 0x41,
	DXGI_FORMAT_R1_UNORM					= 0x42,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP			= 0x43,
	DXGI_FORMAT_R8G8_B8G8_UNORM				= 0x44,
	DXGI_FORMAT_G8R8_G8B8_UNORM				= 0x45,
	DXGI_FORMAT_BC1_TYPELESS				= 0x46,
	DXGI_FORMAT_BC1_UNORM					= 0x47,
	DXGI_FORMAT_BC1_UNORM_SRGB				= 0x48,
	DXGI_FORMAT_BC2_TYPELESS				= 0x49,
	DXGI_FORMAT_BC2_UNORM					= 0x4a,
	DXGI_FORMAT_BC2_UNORM_SRGB				= 0x4b,
	DXGI_FORMAT_BC3_TYPELESS				= 0x4c,
	DXGI_FORMAT_BC3_UNORM					= 0x4d,
	DXGI_FORMAT_BC3_UNORM_SRGB				= 0x4e,
	DXGI_FORMAT_BC4_TYPELESS				= 0x4f,
	DXGI_FORMAT_BC4_UNORM					= 0x50,
	DXGI_FORMAT_BC4_SNORM					= 0x51,
	DXGI_FORMAT_BC5_TYPELESS				= 0x52,
	DXGI_FORMAT_BC5_UNORM					= 0x53,
	DXGI_FORMAT_BC5_SNORM					= 0x54,
	DXGI_FORMAT_B5G6R5_UNORM				= 0x55,
	DXGI_FORMAT_B5G5R5A1_UNORM				= 0x56,
	DXGI_FORMAT_B8G8R8A8_UNORM				= 0x57,
	DXGI_FORMAT_B8G8R8X8_UNORM				= 0x58,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM	= 0x59,
	DXGI_FORMAT_B8G8R8A8_TYPELESS			= 0x5a,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB			= 0x5b,
	DXGI_FORMAT_B8G8R8X8_TYPELESS			= 0x5c,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB			= 0x5d,
	DXGI_FORMAT_BC6H_TYPELESS				= 0x5e,
	DXGI_FORMAT_BC6H_UF16					= 0x5f,
	DXGI_FORMAT_BC6H_SF16					= 0x60,
	DXGI_FORMAT_BC7_TYPELESS				= 0x61,
	DXGI_FORMAT_BC7_UNORM					= 0x62,
	DXGI_FORMAT_BC7_UNORM_SRGB				= 0x63,
	DXGI_FORMAT_FORCE_UINT					= -0x01,
};
enum vostok::render::enum_rt_usage
{
	enum_rt_usage_depth_stencil		= 0x0,
	enum_rt_usage_render_target		= 0x1,
};

class render_target : public resource_intrusive_base {
										render_target			( );
										~render_target			( );

			void						destroy_impl			( ) const;

public:
			void						save_as					( pcstr file_name );

			void						create					(
											pcstr				name,
											u32					width,
											u32					height,
											DXGI_FORMAT			format,
											enum_rt_usage		usage,
											D3D11_USAGE			memory_usage,
											u32					sample_count
										);
			void						create					(
											pcstr				name,
											u32					width,
											u32					height,
											DXGI_FORMAT			format,
											enum_rt_usage		usage,
											res_texture_ptr		in_texture,
											u32					first_array_slice_index,
											u32					mip_slice
										);

			void						create_3d				(
											pcstr				name,
											u32					width,
											u32					height,
											u32					depth,
											DXGI_FORMAT			format,
											enum_rt_usage		usage,
											D3D11_USAGE			memory_usage
										);

			void						destroy					( );

	inline	ID3D11RenderTargetView*		get_target_view			( ) const { /* no source */ }
	inline	ID3D11DepthStencilView*		get_depth_stencil_view	( ) const { /* no source */ }

	inline	bool						valid					( ) { /* no source */ }

	inline	u32							width					( ) { /* no source */ }

	inline	u32							height					( ) { /* no source */ }

	inline	DXGI_FORMAT					format					( ) { /* no source */ }

	inline	enum_rt_usage				usage					( ) { /* no source */ }

	inline	shared_string				name					( ) const { /* no source */ }

	inline	void						set_name				( pcstr const name ) { /* no source */ }

	inline	bool						is_registered			( ) const { /* no source */ }
	inline	void						mark_registered			( ) { /* no source */ }

	inline	res_texture_ptr				get_texture				( ) const { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
private:
	/* 0x0004 */	shared_string				m_name;
	/* 0x0008 */	ID3D11Texture2D*			m_surface;
	/* 0x000c */	ID3D11Texture3D*			m_surface_3d;
	/* 0x0010 */	ID3D11RenderTargetView*		m_rt;
	/* 0x0014 */	ID3D11DepthStencilView*		m_zrt;
	/* 0x0018 */	res_texture_ptr				m_texture;
	/* 0x001c */	u32							m_width;
	/* 0x0020 */	u32							m_height;
	/* 0x0024 */	DXGI_FORMAT					m_format;
	/* 0x0028 */	enum_rt_usage				m_usage;
	/* 0x002c */	u32							m_memory_usage;
	/* 0x0030 */	u64							m_order;
	/* 0x0038 */	bool						m_is_registered;
}; // class render_target

STATIC_SIZE_ASSERT(render_target, 0x40);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_TARGET_H_INCLUDED
