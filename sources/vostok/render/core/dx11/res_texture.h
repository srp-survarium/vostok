#ifndef VOSTOK_RENDER_CORE_DX11_RES_TEXTURE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_TEXTURE_H_INCLUDED

#include <fastdelegate/fastdelegate.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/math_float4.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/resource_intrusive_base.h>

namespace vostok {
namespace render {

class noncopyable_protected {
protected:
	noncopyable_protected( ) { }
	~noncopyable_protected( ) { }
	noncopyable_protected( noncopyable_protected const& ) { }
	noncopyable_protected const& operator=( noncopyable_protected const& ) { return *this; }
};

STATIC_SIZE_ASSERT( noncopyable_protected, 0x1 );

class res_texture :
	public resource_intrusive_base,
	public noncopyable_protected
{
public:
	explicit res_texture( bool pool_texture = false );
	virtual ~res_texture( );

	void destroy_impl( ) const;

	void set_hw_texture(
		ID3D11Resource* surface,
		u32 mip_level_cut = 0,
		bool staging = false,
		bool srgb = false,
		bool depth_stencil = false
	);

	void save_as( pcstr file_name );
	ID3D11Resource* hw_texture( );
	ID3D11ShaderResourceView* view( ) { return m_sh_res_view; }
	D3D11_TEXTURE2D_DESC const& description( ) { desc_enshure( ); return m_desc; }

	u32 width( ) const { desc_enshure( ); return m_desc.Width; }
	u32 height( ) const { desc_enshure( ); return m_desc.Height; }
	u32 mips_count( ) const { desc_enshure( ); return m_desc.MipLevels; }
	u32 array_size( ) const { desc_enshure( ); return m_desc.ArraySize; }
	DXGI_FORMAT format( ) const { desc_enshure( ); return m_desc.Format; }
	bool pool_texture1( ) const { return m_pool_texture; }
	bool is_loaded( ) const { return m_loaded; }
	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( ) { m_is_registered = true; }

	void* map2D(
		D3D11_MAP mode,
		u32 mip_level,
		u32& row_pitch,
		bool do_not_wait = false
	);
	void unmap2D( u32 mip_level );
	void* map3D( D3D11_MAP mode, u32 mip_level, u32& row_pitch );
	void unmap3D( u32 mip_level );

	pcstr name( ) const { return m_name.c_str( ); }
	void set_name( pcstr name )
	{
		// FUNCTION BODY[0x124670]
		m_name = name;
	}
	void clone( res_texture* other );

private:
	void desc_enshure( ) const { }
	void desc_update( );
	D3D11_USAGE get_usage( ) { return D3D11_USAGE_DEFAULT; }

public:
	bool m_loaded;
	u32 num_mips;
	float4 m_rescale_min;
	float4 m_rescale_max;

private:
	u32 m_user : 1;
	u32 m_seq_cycles : 1;
	u32 m_reserved_flags : 30;
	u32 m_mem_usage;
	fastdelegate::FastDelegate1< u32 > m_bind;
	D3D11_TEXTURE2D_DESC m_desc;
	D3D11_TEXTURE3D_DESC m_desc_3d;
	fs_new::virtual_path_string m_name;
	ID3D11Resource* m_surface;
	ID3D11Resource* m_desc_cache_surface;
	ID3D11ShaderResourceView* m_sh_res_view;
	u32 m_mip_level_cut;
	bool m_desc_valid;
	bool m_desc_3d_valid;
	bool m_pool_texture;
	bool m_is_registered;
	bool m_streamed;
};

STATIC_SIZE_ASSERT( res_texture, 0x1BC );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_TEXTURE_H_INCLUDED
