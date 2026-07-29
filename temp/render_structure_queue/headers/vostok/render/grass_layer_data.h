////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_LAYER_DATA_H_INCLUDED
#define RENDER_GRASS_LAYER_DATA_H_INCLUDED

/* INCLUDES */
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
class vostok::resources::managed_resource;

namespace vostok {
namespace render {

struct grass_layer_data {
	inline		grass_layer_data	( ) { /* no source */ }
	inline		~grass_layer_data	( ) { /* no source */ }

	/* 0x0000 */	float								lt_x_m;
	/* 0x0004 */	float								lt_z_m;
	/* 0x0008 */	u16									size_x_cells;
	/* 0x000a */	u16									size_z_cells;
	/* 0x000c */	float								grass_cell_size_m;
	/* 0x0010 */	u8									grass_cells_per_meter;
	/* 0x0014 */	pbyte								stencil_data;
	/* 0x0018 */	float3*								instances_positions;
	/* 0x001c */	float3*								instances_normals;
	/* 0x0020 */	u32									instances_count;
	/* 0x0024 */	resources::managed_resource_ptr		layer_data_raw_file;
}; // struct grass_layer_data

STATIC_SIZE_ASSERT(grass_layer_data, 0x28);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_LAYER_DATA_H_INCLUDED
