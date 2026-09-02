// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_DEVICE_CAPS_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_DEVICE_CAPS_H_INCLUDED
#include <vostok/render/core/memory.h>
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {

class device_caps {
public:
	enum {
		max_gpus = 8
	};

	struct caps_geometry {
		u32 registers : 16;
		u32 instructions : 16;
		u32 software : 1;
		u32 point_sprites : 1;
		u32 vtf : 1;
		u32 npatches : 1;
		u32 clip_planes : 4;
		u32 vertex_cache : 8;
	};

	struct caps_raster {
		u32 registers : 16;
		u32 instructions : 16;
		u32 stages : 4;
		u32 mrt_count : 4;
		u32 b_mrt_mixdepth : 1;
		u32 b_non_pow2 : 1;
		u32 b_cubemap : 1;
	};

	device_caps( );
	void update( );

	bool force_gpu_ref;
	bool force_gpu_sw;
	bool force_gpu_non_pure;
	u32 gpu_num;
	DXGI_FORMAT target_fmt;
	DXGI_FORMAT depth_fmt;
	u32 refresh_rate;
	u16 geometry_major;
	u16 geometry_minor;
	caps_geometry geometry;
	u16 raster_major;
	u16 raster_minor;
	caps_raster raster;
	u32 id_vendor;
	u32 id_device;
};

inline device_caps::device_caps( )
{
	memory::zero( this, sizeof( *this ) );
}

STATIC_SIZE_ASSERT( device_caps::caps_geometry, 0x8 );
STATIC_SIZE_ASSERT( device_caps::caps_raster, 0x8 );
STATIC_SIZE_ASSERT( device_caps, 0x34 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_DEVICE_CAPS_H_INCLUDED
