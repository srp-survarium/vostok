// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "streaming.h"
namespace vostok {
namespace render {

float calculate_streaming_texture_factor(
	float3 const*		positions,
	float2 const*		uvs,
	u32 const			num_vertices,
	u32 const			vertex_stride,
	u16 const*			indices,
	u32 const			num_indices
)
{
	vector< float > texel_ratios;
	texel_ratios.reserve( num_indices / 3 );

	float streaming_factor = 0.f;

	for ( u32 i = 0; i < num_indices / 3; ++i ) {
		u32 const index0 = indices[ i * 3 + 0 ] * vertex_stride;
		u32 const index1 = indices[ i * 3 + 1 ] * vertex_stride;
		u32 const index2 = indices[ i * 3 + 2 ] * vertex_stride;

		float const l1 =
			( *pointer_cast< float3 const* >( pointer_cast< pcbyte >( positions ) + index0 ) -
			  *pointer_cast< float3 const* >( pointer_cast< pcbyte >( positions ) + index1 ) ).length( );

		float const t1 = math::max(
			( *pointer_cast< float2 const* >( pointer_cast< pcbyte >( uvs ) + index0 ) -
			  *pointer_cast< float2 const* >( pointer_cast< pcbyte >( uvs ) + index1 ) ).length( ),
			math::epsilon_5
		);
		float const t2 = math::max(
			( *pointer_cast< float2 const* >( pointer_cast< pcbyte >( uvs ) + index0 ) -
			  *pointer_cast< float2 const* >( pointer_cast< pcbyte >( uvs ) + index2 ) ).length( ),
			math::epsilon_5
		);

		if ( math::abs( t1 * t2 ) > math::epsilon_5 )
			texel_ratios.push_back( math::max( l1 / t1, l1 / t2 ) );
	}

	if ( !texel_ratios.empty( ) ) {
		std::sort( texel_ratios.begin( ), texel_ratios.end( ) );
		streaming_factor = texel_ratios[ u32( texel_ratios.size( ) * .05f ) ];
	}

	return streaming_factor;
}

s32 calculate_needed_texture_mip_levels(
	float4x4 const&		projection_matrix,
	float3 const&		viewer_position,
	math::sphere const&	object_sphere,
	u32 const			screen_size_x,
	u32 const			screen_size_y,
	float const			factor,
	float&				out_distance
)
{
	static u32 const max_texture_size = 4096;
	static u32 const min_streamed_in_mips = 1;
	static u32 const max_streamed_in_mips = 11;
	static u32 quality_index = 0;

	float const distance =
		( viewer_position - object_sphere.center ).length( ) + object_sphere.radius * .25f;

	out_distance = distance;

	if ( distance <= object_sphere.radius )
		return max_streamed_in_mips - quality_index;

	float const screen_space_scale = distance / math::sqrt(
		math::sqr( distance ) - math::sqr( object_sphere.radius )
	);
	float screen_space_size_x = screen_size_x * screen_space_scale * 2.f;
	float screen_space_size_y = screen_size_y * screen_space_scale * 2.f;

	float const screen_space_size =
		math::max( screen_space_size_x, screen_space_size_y ) /
		( object_sphere.radius * 2.f ) * math::max( factor, .0001f );
	u32 const num_mips = math::ceil(
		math::log( math::min( screen_space_size, float( max_texture_size ) ) ) /
		math::log( 2.f )
	) + 1;

	return math::clamp_r( num_mips, min_streamed_in_mips, max_streamed_in_mips ) - quality_index;
}

} // namespace render
} // namespace vostok
