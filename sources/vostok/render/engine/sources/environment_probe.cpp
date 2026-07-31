#include "pch.h"

// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (new-in-target subsystem) - matcher-phase work.
#include "environment_probe.h"

namespace vostok {
namespace render {

environment_probe::environment_probe(
	collision::space_partitioning_tree*	tree,
	environment_probe_properties const&	properties,
	u32									id
) :
	m_properties				( properties ),
	m_aabb						( math::create_zero_aabb( ) ),
	m_id						( id ),
	m_num_mips					( 0 ),
	m_collision_tree			( tree ),
	m_collision_geometry		( 0 ),
	m_collision_object			( 0 ),
	m_occlusion_info_index		( 0 ),
	m_occluded					( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70b550]
}

environment_probe::~environment_probe( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70b060]
}

void environment_probe::remove_collision( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70af40]
}

bool environment_probe::is_occluded( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x70b040]
	return false;
}

u32 calc_mip_map_count( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70afe0]
	return 0;
}

void environment_probe::set_properties(
	environment_probe_properties const& in_properties
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x70b0a0]
	m_properties = in_properties;
}

} // namespace render
} // namespace vostok
