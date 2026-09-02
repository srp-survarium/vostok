// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_CLOUD_SIMULATION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CLOUD_SIMULATION_H_INCLUDED
#include <boost/noncopyable.hpp>
#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/facade/cloud_key.h>

namespace vostok {
namespace render {

class cloud_simulation : boost::noncopyable {
public:
	struct voxel {
		u8 x;
		u8 y;
		u8 z;
		u8 w;
	};

	cloud_simulation( u32 const in_size_x, u32 const in_size_y, u32 const in_size_z );
	~cloud_simulation( );

	voxel* get_voxels( ) const;
	void generate( cloud_key_parameters const& init_key, float3 const& sun_direction );

	float3		cloud_offset;
	float4x4	world_to_cloud;
	float		interp_alpha;

	void copy_from( cloud_simulation const& other );

private:
	void set_voxel( voxel const& v, u32 const x, u32 const y, u32 const z );

	void set_voxel( voxel* voxels, voxel const& v, u32 const x, u32 const y, u32 const z )
	{
		voxels[(z * m_clouds_size_y + y) * m_clouds_size_x + x] = v;
	}

	voxel const& get_voxel( u32 const x, u32 const y, u32 const z ) const;

	voxel const& get_voxel( voxel* voxels, u32 const x, u32 const y, u32 const z ) const
	{
		return voxels[(z * m_clouds_size_y + y) * m_clouds_size_x + x];
	}

	voxel get_interp_voxel( float, float, float ) const
	{
		// STATE[STUB]
		voxel result = { 0, 0, 0, 0 };
		return result;
	}

	voxel get_interp_voxel2( float, float, float ) const
	{
		// STATE[STUB]
		voxel result = { 0, 0, 0, 0 };
		return result;
	}

	bool is_empty( u32 const x, u32 const y, u32 const z ) const;
	bool in_grid( u32 const y, u32 const z, u32 const arg_2 ) const;

	void smooth_transparency( );
	void smooth_voxels_around( );
	void fill_default_volume( );

	float get_cloud_density( u32 const, u32 const, u32 const ) { return 0.0f; }
	float get_cloud_density_near( u32 const, u32 const, u32 const ) { return 0.0f; }
	float get_cloud_density_interp( float const, float const, float const ) { return 0.0f; }

	void compute_direct_light( float3 const& sun_direction, cloud_key_parameters const& init_key );

	void compute_ambient_light( float3 const&, cloud_key_parameters const& )
	{
		// STATE[STUB]
	}

	void compute_indirect_light( float3 const& sun_direction, cloud_key_parameters const& init_key );

	void compute_ambient_occlusion( cloud_key_parameters const& )
	{
		// STATE[STUB]
	}

	void compute_cloud_density( );

	voxel*		m_voxels;
	float*		m_densities;
	u32 const	m_clouds_size_x;
	u32 const	m_clouds_size_y;
	u32 const	m_clouds_size_z;
};

STATIC_SIZE_ASSERT( cloud_simulation::voxel, 0x4 );
STATIC_SIZE_ASSERT( cloud_simulation, 0x64 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CLOUD_SIMULATION_H_INCLUDED
