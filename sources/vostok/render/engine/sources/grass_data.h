// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_GRASS_DATA_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_GRASS_DATA_H_INCLUDED
#include <vostok/fixed_string.h>
#include <vostok/fixed_vector.h>
#include <vostok/math_float3.h>
#include <vostok/resources_managed_resource.h>

namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace resources {

class query_result_for_cook;

} // namespace resources

namespace render {

struct grass_world;

struct grass_layer_desc {
	u8 id;
	float max_slope_ang;
	bool use_face_normal;
	bool random_orient;
	float random_scale;
	float wind_factor;

	struct model_desc {
		fixed_string<260> name;
		float probability_;
		float scale;
	};

	STATIC_SIZE_ASSERT( model_desc, 0x118 );

	fixed_vector<model_desc, 16> models_list;
};

STATIC_SIZE_ASSERT( grass_layer_desc, 0x119C );

struct grass_layer_data {
	grass_layer_data( ) :
		lt_x_m( 0.f ),
		lt_z_m( 0.f ),
		size_x_cells( 0 ),
		size_z_cells( 0 ),
		grass_cell_size_m( 1.f ),
		grass_cells_per_meter( 1 ),
		stencil_data( 0 ),
		instances_positions( 0 ),
		instances_normals( 0 ),
		instances_count( 0 )
	{
	}

	float lt_x_m;
	float lt_z_m;
	u16 size_x_cells;
	u16 size_z_cells;
	float grass_cell_size_m;
	u8 grass_cells_per_meter;
	pbyte stencil_data;
	float3* instances_positions;
	float3* instances_normals;
	u32 instances_count;
	resources::managed_resource_ptr layer_data_raw_file;
};

STATIC_SIZE_ASSERT( grass_layer_data, 0x28 );

struct grass_loading_data {
	configs::binary_config_value const* t_current;
	fixed_string<260> project_resources_path;
};

STATIC_SIZE_ASSERT( grass_loading_data, 0x114 );

struct grass_cook_data {
	grass_layer_desc** desc;
	grass_layer_data** data;
	resources::query_result_for_cook* parent_query;
	grass_world* result;
};

STATIC_SIZE_ASSERT( grass_cook_data, 0x10 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_GRASS_DATA_H_INCLUDED
