// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_DEVICE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_DEVICE_H_INCLUDED
#include <vostok/render/api.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {

class device : public quasi_singleton< device > {
public:
	explicit device( bool is_editor );
	~device( );

	ID3D11Device* d3d_device( ) const;
	ID3D11DeviceContext* d3d_context( ) const;
	IDXGIOutput* get_output( u32 const monitor_index ) const;

	void on_device_removed( );

	bool get_query_data(
		ID3D11Query* in_query,
		void* in_out_data,
		u32 const in_data_size,
		bool in_wait
	);

	D3D_FEATURE_LEVEL get_feature_level( ) const { return m_feature_level; }
	u32 get_avaliable_video_memory( ) const;
	u32 get_max_texture_size( ) const
	{
		// claude@NOTE: no body is available in the developer sources.
		// STATE[STUB]
		return 0;
	}
	bool get_is_editor( ) const { return m_is_editor; }

private:
	void create_d3d( );
	void destroy_d3d( );
	void create( );
	void destroy( );

private:
	DXGI_ADAPTER_DESC m_adapter_desc;
	D3D_FEATURE_LEVEL m_feature_level;
	IDXGIAdapter* m_adapter;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	IDXGIOutput* m_outputs[6];

public:
	bool m_is_editor;
	bool m_use_perfhud;
	bool m_device_removed;
	u32 m_avaliable_video_memory;
};

STATIC_SIZE_ASSERT( device, 0x154 );

// Filled during adapter-output enumeration and read by the video options page.
extern VOSTOK_RENDER_API int g_num_monitors;
extern VOSTOK_RENDER_API math::int2 g_monitor_resolutions[6][512];

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_DEVICE_H_INCLUDED
