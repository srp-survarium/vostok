#include "pch.h"
#include "cloud_simulation.h"

namespace vostok {
namespace render {

cloud_simulation::cloud_simulation(
	u32 in_size_x,
	u32 in_size_y,
	u32 in_size_z
) :
	cloud_offset		( 0.0f, 0.0f, 0.0f ),
	interp_alpha		( 0.0f ),
	m_voxels			( 0 ),
	m_densities		( 0 ),
	m_clouds_size_x	( in_size_x ),
	m_clouds_size_y	( in_size_y ),
	m_clouds_size_z	( in_size_z )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62fd70]
}

cloud_simulation::~cloud_simulation( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62fd30]
}

void cloud_simulation::copy_from( cloud_simulation const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62f1b0]
}

cloud_simulation::voxel* cloud_simulation::get_voxels( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ec40]
	return 0;
}

void cloud_simulation::set_voxel( voxel const&, u32, u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ec70]
}

cloud_simulation::voxel const& cloud_simulation::get_voxel( u32, u32, u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ec50]
	return *m_voxels;
}

void cloud_simulation::compute_cloud_density( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ee80]
}

bool cloud_simulation::is_empty( u32, u32, u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ee50]
	return false;
}

bool cloud_simulation::in_grid( u32, u32, u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ec20]
	return false;
}

void cloud_simulation::smooth_transparency( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62f910]
}

void cloud_simulation::compute_direct_light(
	float3 const&,
	cloud_key_parameters const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ed30]
}

void cloud_simulation::compute_indirect_light(
	float3 const&,
	cloud_key_parameters const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x62f1e0]
}

void cloud_simulation::smooth_voxels_around( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ec10]
}

void cloud_simulation::fill_default_volume( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62eca0]
}

void cloud_simulation::generate(
	cloud_key_parameters const&,
	float3 const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x62fae0]
}

} // namespace render
} // namespace vostok
