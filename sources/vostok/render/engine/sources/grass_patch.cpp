#include "pch.h"
#include "grass_patch.h"

namespace vostok {
namespace render {

bool has_surface_by_lod( grass_render_model_ptr model, u32 lod_index )
{
	// FUNCTION BODY[0x5f43e0]
	return surface_by_lod( model, lod_index ) != 0;
}

grass_render_surface* surface_by_lod( grass_render_model_ptr model, u32 lod_index )
{
	// FUNCTION BODY[0x5f4310]
	if ( !model )
		return 0;

	switch ( lod_index ) {
	case 0: return model->m_l0;
	case 1: return model->m_l1;
	case 2: return model->m_l2;
	default: return 0;
	}
}

grass_patch::grass_patch(
	collision::space_partitioning_tree* in_collision_tree,
	grass_template* templ,
	float3 const& in_origin,
	float in_size
) :
	m_prev_view_pos( 0.f, 0.f, 0.f ),
	m_aabb( math::create_zero_aabb( ) ),
	m_origin( in_origin ),
	m_size( in_size ),
	m_occlusion_info_index( 0 ),
	m_current_lod_index( 0 ),
	m_num_avaliable_lods( 0 ),
	m_template( templ ),
	m_collision_tree( in_collision_tree ),
	m_collision_geometry( 0 ),
	m_collision_object( 0 ),
	m_visible( false ),
	m_occluded( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f5280]
	for ( u32 i = 0; i != 3; ++i ) {
		m_merged_indices[i] = 0;
		m_sort_info[i] = 0;
		m_num_merged_vertices[i] = 0;
		m_num_merged_indices[i] = 0;
	}
}

void grass_patch::remove_trample( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f3b10]
}

void grass_patch::try_accumulate_trample( trample_desc&, grass_world*, renderer*, renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f5060]
}

grass_patch::~grass_patch( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f4480]
}

void grass_patch::init_collision( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f3ba0]
}

u32 grass_patch::get_stream_1_stride( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f3ae0]
	return 0;
}

u32 grass_patch::get_index_count( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f3ad0]
	return 0;
}

u32 grass_patch::get_valid_lod_index( u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f3ab0]
	return 0;
}

void grass_patch::render(
	grass_world*,
	renderer_context*,
	float3 const&,
	enum_render_stage_type,
	u32,
	float,
	res_effect*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f4df0]
}

void grass_patch::sort_instances( float3 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f41d0]
}

math::color transform_packed_normal( float4x4 const&, math::color const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f40a0]
	return math::color( );
}

void grass_patch::merge_instances( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f4700]
}

bool grass_patch::is_occluded( ) const
{
	// FUNCTION BODY[0x5f3af0]
	return m_occluded;
}

} // namespace render
} // namespace vostok
