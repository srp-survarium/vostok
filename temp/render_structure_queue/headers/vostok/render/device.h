////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DEVICE_H_INCLUDED
#define RENDER_DEVICE_H_INCLUDED

/* INCLUDES */
struct DXGI_ADAPTER_DESC;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGIAdapter;
struct IDXGIOutput;
class vostok::quasi_singleton<vostok::render::device>;

namespace vostok {
namespace render {

enum D3D_FEATURE_LEVEL
{
	D3D_FEATURE_LEVEL_9_1		= 0x9100,
	D3D_FEATURE_LEVEL_9_2		= 0x9200,
	D3D_FEATURE_LEVEL_9_3		= 0x9300,
	D3D_FEATURE_LEVEL_10_0		= 0xa000,
	D3D_FEATURE_LEVEL_10_1		= 0xa100,
	D3D_FEATURE_LEVEL_11_0		= 0xb000,
};

class device : public quasi_singleton< device > {
public:
			explicit				device						( bool is_editor );
									~device						( );

			ID3D11Device*			d3d_device					( ) const;
			ID3D11DeviceContext*	d3d_context					( ) const;

			IDXGIOutput*			get_output					( const u32 monitor_index ) const;

			void					on_device_removed			( );

			bool					get_query_data				(
										ID3D11Query*	in_query,
										void*			in_out_data,
										const u32		in_data_size,
										bool			in_wait
									);
	inline	D3D_FEATURE_LEVEL		get_feature_level			( ) const { /* no source */ }
			u32						get_avaliable_video_memory	( ) const;
	inline	u32						get_max_texture_size		( ) const { /* no source */ }
	inline	bool					get_is_editor				( ) const { /* no source */ }

private:
			void					create_d3d					( );
			void					destroy_d3d					( );

			void					create						( );

			void					destroy						( );

	/* 0x0000 */	/* quasi_singleton< device > */
	/* 0x0000 */	DXGI_ADAPTER_DESC		m_adapter_desc;
	/* 0x0124 */	D3D_FEATURE_LEVEL		m_feature_level;
	/* 0x0128 */	IDXGIAdapter*			m_adapter;
	/* 0x012c */	ID3D11Device*			m_device;
	/* 0x0130 */	ID3D11DeviceContext*	m_context;
	/* 0x0134 */	IDXGIOutput*			m_outputs[6];
public:
	/* 0x014c */	bool					m_is_editor;
	/* 0x014d */	bool					m_use_perfhud;
	/* 0x014e */	bool					m_device_removed;
	/* 0x0150 */	u32						m_avaliable_video_memory;
}; // class device

STATIC_SIZE_ASSERT(device, 0x154);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DEVICE_H_INCLUDED
