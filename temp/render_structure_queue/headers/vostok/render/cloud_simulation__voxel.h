////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CLOUD_SIMULATION_VOXEL_H_INCLUDED
#define RENDER_CLOUD_SIMULATION_VOXEL_H_INCLUDED

namespace vostok {
namespace render {

struct cloud_simulation::voxel {

	/* 0x0000 */	u8		x;
	/* 0x0001 */	u8		y;
	/* 0x0002 */	u8		z;
	/* 0x0003 */	u8		w;
}; // struct cloud_simulation::voxel

STATIC_SIZE_ASSERT(cloud_simulation::voxel, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CLOUD_SIMULATION_VOXEL_H_INCLUDED
