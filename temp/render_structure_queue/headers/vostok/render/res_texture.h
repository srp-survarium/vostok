////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_TEXTURE_H_INCLUDED
#define RENDER_RES_TEXTURE_H_INCLUDED

/* INCLUDES */
class fastdelegate::FastDelegate1<unsigned int,void>;
struct D3D11_TEXTURE2D_DESC;
struct D3D11_TEXTURE3D_DESC;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;
class vostok::fs_new::virtual_path_string;
class vostok::render::noncopyable_protected;
class vostok::render::resource_intrusive_base;

namespace vostok {
namespace render {

class res_texture : public resource_intrusive_base , public noncopyable_protected {
			explicit						res_texture		( bool pool_texture );
	virtual									~res_texture	( );

			void							destroy_impl	( ) const;

			void							set_hw_texture	(
												ID3D11Resource*		surface,
												u32					mip_level_cut,
												bool				staging,
												bool				srgb,
												bool				depth_stencil
											);

public:
			void							save_as			( pcstr file_name );

			ID3D11Resource*					hw_texture		( );

	inline	ID3D11ShaderResourceView*		view			( ) { /* no source */ }

	inline	D3D11_TEXTURE2D_DESC const&		description		( ) { /* no source */ }

	inline	u32								width			( ) const { /* no source */ }

	inline	u32								height			( ) const { /* no source */ }

	inline	u32								mips_count		( ) const { /* no source */ }

	inline	u32								array_size		( ) const { /* no source */ }

	inline	DXGI_FORMAT						format			( ) const { /* no source */ }

	inline	bool							pool_texture1	( ) const { /* no source */ }

	inline	bool							is_loaded		( ) const { /* no source */ }
	inline	bool							is_registered	( ) const { /* no source */ }
	inline	void							mark_registered	( ) { /* no source */ }

			void*							map2D			(
												D3D11_MAP		mode,
												u32				mip_level,
												u32&			row_pitch,
												bool			dot_not_wait
											);

			void							unmap2D			( u32 mip_level );

			void*							map3D			( D3D11_MAP mode, u32 mip_level, u32& row_pitch );

			void							unmap3D			( u32 mip_level );

	inline	pcstr							name			( ) const { /* no source */ }

	inline	void							set_name		( pcstr const name ) { /* no source */ }

			void							clone			( res_texture* other );

private:
	inline	void							desc_enshure	( ) const { /* no source */ }
			void							desc_update		( );

	inline	D3D11_USAGE						get_usage		( ) { /* no source */ }

	/* 0x0004 */	/* resource_intrusive_base */
	/* 0x0008 */	/* noncopyable_protected */
public:
	/* 0x0008 */	bool							m_loaded;
	/* 0x000c */	u32								num_mips;
	/* 0x0010 */	float4							m_rescale_min;
	/* 0x0020 */	float4							m_rescale_max;
private:
	/* 0x0030 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 0 } )*/	m_user;
	/* 0x0030 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 1 } )*/	m_seq_cycles;
	/* 0x0034 */	u32								m_mem_usage;
	/* 0x0038 */	fastdelegate::FastDelegate1< u32, void >	m_bind;
	/* 0x0040 */	D3D11_TEXTURE2D_DESC			m_desc;
	/* 0x006c */	D3D11_TEXTURE3D_DESC			m_desc_3d;
	/* 0x0090 */	fs_new::virtual_path_string		m_name;
	/* 0x01a4 */	ID3D11Resource*					m_surface;
	/* 0x01a8 */	ID3D11Resource*					m_desc_cache_surface;
	/* 0x01ac */	ID3D11ShaderResourceView*		m_sh_res_view;
	/* 0x01b0 */	u32								m_mip_level_cut;
	/* 0x01b4 */	bool							m_desc_valid;
	/* 0x01b5 */	bool							m_desc_3d_valid;
	/* 0x01b6 */	bool							m_pool_texture;
	/* 0x01b7 */	bool							m_is_registered;
	/* 0x01b8 */	bool							m_streamed;
}; // class res_texture

STATIC_SIZE_ASSERT(res_texture, 0x1BC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_TEXTURE_H_INCLUDED
