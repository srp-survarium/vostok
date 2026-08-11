#include "pch.h"

#include <vostok/render/core/options.h>

#include "cloud_noise.h"
#include "cloud_simulation.h"

namespace vostok {
namespace render {

cloud_simulation::cloud_simulation(
	u32 const in_size_x,
	u32 const in_size_y,
	u32 const in_size_z
) :
	m_clouds_size_x	( in_size_x ),
	m_clouds_size_y	( in_size_y ),
	m_clouds_size_z	( in_size_z )
{
	// 15 target lines are likely retail-compiled-out source.
	m_voxels = NEW_ARRAY( voxel, m_clouds_size_x * m_clouds_size_y * m_clouds_size_z );
	m_densities = NEW_ARRAY( float, m_clouds_size_x * m_clouds_size_y * m_clouds_size_z );
	cloud_offset = float3( 0.0f, 0.0f, 0.0f );
	world_to_cloud = float4x4( ).identity( );
	interp_alpha = 0.0f;
}

cloud_simulation::~cloud_simulation( )
{
	DELETE_ARRAY( m_voxels );
	DELETE_ARRAY( m_densities );
}

void cloud_simulation::copy_from( cloud_simulation const& other )
{
	memory::copy( m_voxels, m_clouds_size_x * m_clouds_size_y * m_clouds_size_z * sizeof(voxel),
		other.m_voxels, m_clouds_size_x * m_clouds_size_y * m_clouds_size_z * sizeof(voxel) );
}

cloud_simulation::voxel* cloud_simulation::get_voxels( ) const
{
	return m_voxels;
}

void cloud_simulation::set_voxel(
	voxel const& v,
	u32 const x,
	u32 const y,
	u32 const z
) {
	m_voxels[(z * m_clouds_size_y + y) * m_clouds_size_x + x] = v;
}

cloud_simulation::voxel const& cloud_simulation::get_voxel(
	u32 const x,
	u32 const y,
	u32 const z
) const
{
	return m_voxels[(z * m_clouds_size_y + y) * m_clouds_size_x + x];
}

void cloud_simulation::compute_cloud_density( )
{
	// 18 target lines are likely retail-compiled-out source.
	for ( u32 z = 0; z < m_clouds_size_z; ++z )
	{
		for ( u32 x = 0; x < m_clouds_size_x; ++x )
		{
			for ( u32 y = 1; y < m_clouds_size_y - 1; ++y )
			{

				float& out_density = m_densities[(z * m_clouds_size_y + y) * m_clouds_size_x + x];

				if ( is_empty( x, y, z ) )
				{
					out_density = 0.0f;
					continue;
				}
				// 4 target lines are likely retail-compiled-out source.
				float accumulated = 0.0f;

				float num = 0.0f;
				// 5 target lines are likely retail-compiled-out source.
				for ( s32 x0 = -2; x0 <= 2; ++x0 )
				{
					for ( s32 y0 = -2; y0 <= 2; ++y0 )
					{
						for ( s32 z0 = -2; z0 <= 2; ++z0 )
						{
							++num;

							u32 sample_x = x + x0; u32 const sample_y = y + y0; u32 sample_z = z + z0;

							if ( sample_x >= m_clouds_size_x || !in_grid( sample_y, sample_z, sample_x ) ) {
								sample_x %= m_clouds_size_x;

								sample_z %= m_clouds_size_z;
							}
							// 5 target lines are likely retail-compiled-out source.
							if ( sample_x < m_clouds_size_x && in_grid( sample_y, sample_z, sample_x ) ) accumulated += get_voxel( sample_x, sample_y, sample_z ).x / 255.0f;
						}
					}
				}
				// 3 target lines are likely retail-compiled-out source.
				if ( num > 0.0f )
					out_density = accumulated / num;
				else
					out_density = 0.0f;
			}
		}
	}
}
// 110 target lines are likely retail-compiled-out source.
bool cloud_simulation::is_empty( u32 const x, u32 const y, u32 const z ) const
{
	return get_voxel( x, y, z ).x == 0;
}

bool cloud_simulation::in_grid( u32 const y, u32 const z, u32 const ) const
{
	// 2 target lines are likely retail-compiled-out source.
	return y < m_clouds_size_y &&
		z < m_clouds_size_z;
}

void cloud_simulation::smooth_transparency( ) {
	for ( u32 z = 0; z < m_clouds_size_z; ++z )
	{
		for ( u32 x = 0; x < m_clouds_size_x; ++x )
		{
			for ( u32 y = 1; y < m_clouds_size_y - 1; ++y )
			{
				voxel v = get_voxel( x, y, z );
				// 6 target lines are likely retail-compiled-out source.
				float transparency = static_cast<float>( v.x );

				if ( x - 1 < m_clouds_size_x && in_grid( y, z, x - 1 ) && is_empty( x - 1, y, z ) )
					transparency *= 0.65f;

				if ( x + 1 < m_clouds_size_x && in_grid( y, z, x + 1 ) && is_empty( x + 1, y, z ) )
					transparency *= 0.65f;

				if ( x < m_clouds_size_x && in_grid( y - 1, z, x ) && is_empty( x, y - 1, z ) )
					transparency *= 0.65f;

				if ( x < m_clouds_size_x && in_grid( y + 1, z, x ) && is_empty( x, y + 1, z ) )
					transparency *= 0.65f;

				if ( x < m_clouds_size_x && in_grid( y, z - 1, x ) && is_empty( x, y, z - 1 ) )
					transparency *= 0.65f;

				if ( x < m_clouds_size_x && in_grid( y, z + 1, x ) && is_empty( x, y, z + 1 ) )
					transparency *= 0.65f;

				v.x = static_cast<u8>( math::clamp_r( transparency, 0.0f, 255.0f ) );
				set_voxel( v, x, y, z );
			}
		}
	}
}

void cloud_simulation::compute_direct_light(
	float3 const& sun_direction,
	cloud_key_parameters const& init_key
)
{
	// 65 target lines are likely retail-compiled-out source.
	for ( u32 z = 0; z < m_clouds_size_z; ++z )
	{
		for ( u32 y = 0; y < m_clouds_size_y; ++y )
		{
			for ( u32 x = 0; x < m_clouds_size_x; ++x )
			{
				voxel v = get_voxel( x, y, z );
				v.z = static_cast<u8>( math::clamp_r( math::pow( math::pow( v.y / 255.0f, 16.0f ), init_key.extinction ) * 4.0f, 0.0f, 1.0f ) * 255.0f );
				set_voxel( v, x, y, z );
			}
		}
	}
}

void cloud_simulation::compute_indirect_light(
	float3 const& sun_direction,
	cloud_key_parameters const& init_key
)
{
	float const vertical_direction = math::clamp_r( math::abs( sun_direction | float3( 0.0f, 1.0f, 0.0f ) ), 0.0f, 1.0f );

	u32 num = static_cast<u32>( (1.0f - vertical_direction) * static_cast<float>( m_clouds_size_y * 6 ) + vertical_direction * static_cast<float>( m_clouds_size_y ) );

	for ( u32 z = 0; z < m_clouds_size_z; ++z )
	{
		for ( u32 x = 0; x < m_clouds_size_x; ++x )
		{
			for ( u32 y = 0; y < m_clouds_size_y; ++y )
			{
				voxel v = get_voxel( x, y, z );

				if ( v.x )
				{
					float accumulated = 0.0f;

					for ( u32 i = 0; i < num; ++i )
					{
						float3 coord =
							float3( static_cast<float>( x ), static_cast<float>( y ), static_cast<float>( z ) ) +
							sun_direction * static_cast<float>( i + 1 );

						if ( static_cast<u32>( math::max( coord.x, 0.0f ) ) >= m_clouds_size_x ||
							!in_grid(
								static_cast<u32>( math::max( coord.y, 0.0f ) ),
								static_cast<u32>( math::max( coord.z, 0.0f ) ),
								static_cast<u32>( math::max( coord.x, 0.0f ) )
							)
						)
						{
							if ( math::floor( coord.x ) < m_clouds_size_x &&
								math::floor( coord.y ) > m_clouds_size_y &&
								math::floor( coord.z ) < m_clouds_size_z )
							{
								accumulated += 1.0f;
								continue;
							}

							coord.x = static_cast<float>( math::floor( coord.x ) % m_clouds_size_x );
							coord.z = static_cast<float>( math::floor( coord.z ) % m_clouds_size_z );
						}

						if ( static_cast<u32>( math::max( coord.x, 0.0f ) ) >= m_clouds_size_x ||
							!in_grid(
								static_cast<u32>( math::max( coord.y, 0.0f ) ),
								static_cast<u32>( math::max( coord.z, 0.0f ) ),
								static_cast<u32>( math::max( coord.x, 0.0f ) )
							)
						)
							break;

						if ( is_empty(
							static_cast<u32>( math::max( coord.x, 0.0f ) ),
							static_cast<u32>( math::max( coord.y, 0.0f ) ),
							static_cast<u32>( math::max( coord.z, 0.0f ) )
						) )
							break;

						accumulated += 1.0f;
					}

					accumulated /= static_cast<float>( num );

					accumulated = math::clamp_r( accumulated, 0.0f, 1.0f );

					v.y = static_cast<u8>(
						math::clamp_r(
							math::pow( 1.0f - accumulated, init_key.extinction ),
							0.0f,
							1.0f
						) * 255.0f
					);
					set_voxel( v, x, y, z );
				}
			}
		}
	}
}

void cloud_simulation::smooth_voxels_around( )
{
}

void cloud_simulation::fill_default_volume( )
{
	memory::zero(
		m_voxels,
		m_clouds_size_x * m_clouds_size_y * m_clouds_size_z * sizeof(voxel)
	);

	for ( u32 z = 0; z < m_clouds_size_z; ++z )
	{
		for ( u32 x = 0; x < m_clouds_size_x; ++x )
		{
			for ( u32 y = 0; y < m_clouds_size_y; ++y )
			{
				voxel v;
				v.x = 0;
				v.y = 0;
				v.z = 0;
				v.w = 0;
				set_voxel( v, x, y, z );
			}
		}
	}
}

void cloud_simulation::generate(
	cloud_key_parameters const& init_key,
	float3 const& sun_direction
)
{
	float3 to_sun_direction = sun_direction;

	fill_default_volume( );

	float const cloudiness = math::clamp_r(
		1.0f - init_key.cloud_generate_cloudiness + 0.05f,
		0.0f,
		1.0f
	);
	float const cloudiness2 = math::clamp_r(
		1.0f - init_key.cloud_generate_cloudiness + 0.1f,
		0.0f,
		1.0f
	);
	u32 const num_octaves = math::floor( init_key.cloud_generate_octaves );
	float const cloudiness3 = 1.0f - init_key.cloud_generate_cloudiness;

	for ( u32 z = 0; z < m_clouds_size_z; ++z )
	{
		for ( u32 x = 0; x < m_clouds_size_x; ++x )
		{
			float const noise = cloud_noise::evaluate(
				static_cast<float>( z ) / static_cast<float>( m_clouds_size_z ),
				static_cast<float>( x ) / static_cast<float>( m_clouds_size_x ),
				num_octaves
			);

			u32 min_y = 1;
			u32 max_y = 1;

			if ( noise - cloudiness2 > 0.0f )
			{
				max_y = m_clouds_size_y - 1;
			}
			else if ( noise - cloudiness > 0.0f )
			{
				min_y = 2;
				max_y = m_clouds_size_y - 3;
			}
			else if ( noise - cloudiness3 > 0.0f )
			{
				min_y = 3;
				max_y = m_clouds_size_y - 4;
			}

			if ( max_y - min_y > 1 && min_y < max_y )
			{
				for ( u32 y = min_y; y < max_y; ++y )
				{
					voxel v = get_voxel( x, y, z );
					v.x = static_cast<u8>(
						math::clamp_r( 1.0f - init_key.diffusivity, 0.0f, 1.0f ) * 255.0f
					);
					set_voxel( v, x, y, z );
				}
			}
		}
	}

	if ( !options::ref( ).current.m_use_shader_lods )
	{
		compute_cloud_density( );
		compute_indirect_light( to_sun_direction, init_key );
		compute_direct_light( sun_direction, init_key );
		smooth_transparency( );
	}
}

} // namespace render
} // namespace vostok
