#include "pch.h"
#include <vostok/render/core/res_texture.h>

namespace vostok {
namespace render {

// STATE[STUB]
res_texture::res_texture( bool pool_texture ) :
	m_loaded( false ),
	num_mips( 0 ),
	m_user( 0 ),
	m_seq_cycles( 0 ),
	m_reserved_flags( 0 ),
	m_mem_usage( 0 ),
	m_surface( 0 ),
	m_desc_cache_surface( 0 ),
	m_sh_res_view( 0 ),
	m_mip_level_cut( 0 ),
	m_desc_valid( false ),
	m_desc_3d_valid( false ),
	m_pool_texture( pool_texture ),
	m_is_registered( false ),
	m_streamed( false )
{
	// FUNCTION BODY[0x55c5b0]
}

// STATE[STUB]
res_texture::~res_texture( )
{
	// FUNCTION BODY[0x55c430]
}

// STATE[STUB]
void res_texture::destroy_impl( ) const
{
	// FUNCTION BODY[0x55c670]
}

// STATE[STUB]
void res_texture::save_as( pcstr file_name )
{
	VOSTOK_UNREFERENCED_PARAMETER( file_name );
	// FUNCTION BODY[0x55c300]
}

// STATE[STUB]
void res_texture::desc_update( )
{
	// FUNCTION BODY[0x55c2a0]
}

// STATE[STUB]
void res_texture::set_hw_texture(
	ID3D11Resource* surface,
	u32 mip_level_cut,
	bool staging,
	bool srgb,
	bool depth_stencil
)
{
	VOSTOK_UNREFERENCED_PARAMETER( surface );
	VOSTOK_UNREFERENCED_PARAMETER( mip_level_cut );
	VOSTOK_UNREFERENCED_PARAMETER( staging );
	VOSTOK_UNREFERENCED_PARAMETER( srgb );
	VOSTOK_UNREFERENCED_PARAMETER( depth_stencil );
	// FUNCTION BODY[0x55c6c0]
}

// STATE[STUB]
void* res_texture::map2D(
	D3D11_MAP mode,
	u32 mip_level,
	u32& row_pitch,
	bool do_not_wait
)
{
	VOSTOK_UNREFERENCED_PARAMETER( mode );
	VOSTOK_UNREFERENCED_PARAMETER( mip_level );
	VOSTOK_UNREFERENCED_PARAMETER( row_pitch );
	VOSTOK_UNREFERENCED_PARAMETER( do_not_wait );
	// FUNCTION BODY[0x55c530]
	return 0;
}

// STATE[STUB]
void res_texture::unmap2D( u32 mip_level )
{
	VOSTOK_UNREFERENCED_PARAMETER( mip_level );
	// FUNCTION BODY[0x55c510]
}

// STATE[STUB]
void* res_texture::map3D( D3D11_MAP mode, u32 mip_level, u32& row_pitch )
{
	VOSTOK_UNREFERENCED_PARAMETER( mode );
	VOSTOK_UNREFERENCED_PARAMETER( mip_level );
	VOSTOK_UNREFERENCED_PARAMETER( row_pitch );
	// FUNCTION BODY[0x55c4a0]
	return 0;
}

// STATE[STUB]
void res_texture::unmap3D( u32 mip_level )
{
	VOSTOK_UNREFERENCED_PARAMETER( mip_level );
	// FUNCTION BODY[0x55c480]
}

// STATE[STUB]
ID3D11Resource* res_texture::hw_texture( )
{
	// FUNCTION BODY[0x55c290]
	return m_surface;
}

// STATE[STUB]
void res_texture::clone( res_texture* other )
{
	VOSTOK_UNREFERENCED_PARAMETER( other );
	// FUNCTION BODY[0x55c310]
}

} // namespace render
} // namespace vostok
