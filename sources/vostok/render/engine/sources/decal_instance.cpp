#include "pch.h"
#include "decal_instance.h"

namespace vostok {
namespace render {

decal_shader_constants_and_geometry::decal_shader_constants_and_geometry( ) :
	m_world_to_decal_parameter						( 0 ),
	m_eye_ray_corner_parameter						( 0 ),
	m_decal_tangent_to_view_space_matrix_parameter	( 0 ),
	m_decal_angle_parameters						( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x641550]
}

void decal_shader_constants_and_geometry::set(
	renderer_context*,
	float4x4 const&,
	float4x4 const&,
	float,
	float,
	float3 const&,
	float4x4 const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x640b30]
}

void decal_shader_constants_and_geometry::set_geometry( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x640b20]
}

void decal_shader_constants_and_geometry::create_decal_geometry( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x641450]
}

decal_instance::decal_instance(
	collision::space_partitioning_tree*	tree,
	decal_properties const&				properties,
	u32									id
) :
	m_aabb						( math::create_zero_aabb( ) ),
	m_id						( id ),
	m_collision_tree			( tree ),
	m_collision_geometry		( 0 ),
	m_collision_object			( 0 ),
	m_occlusion_info_index		( 0 ),
	m_draw_priority				( properties.draw_priority ),
	m_occluded					( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6412b0]
	set_properties( properties );
}

decal_instance::~decal_instance( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x640ef0]
}

void decal_instance::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x640f90]
}

void decal_instance::remove_collision( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x640a60]
}

bool decal_instance::is_occluded( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x640b00]
	return false;
}

void decal_instance::set_materail_effects( resources::unmanaged_resource_ptr const& in_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x640f30]
	m_properties.material = in_ptr;
}

void decal_instance::set_properties( decal_properties const& in_properties )
{
	// STATE[STUB]
	// FUNCTION BODY[0x641000]
	m_properties = in_properties;
	m_draw_priority = in_properties.draw_priority;
}

decal_properties const& decal_instance::get_properties( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x640a50]
	return m_properties;
}

material_effects const& decal_instance::get_effects( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x640fc0]
	return *static_cast<material_effects const*>( 0 );
}

static float4x4 get_decal_view_matrix( decal_instance* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x640dc0]
	return float4x4( );
}

static float4x4 get_world_to_decal_matrix( decal_instance* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x640c20]
	return float4x4( );
}

void decal_instance::render_geometry( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6413b0]
}

void decal_instance::render( renderer_context*, enum_render_stage_type )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6416e0]
}

u32 decal_instance::draw(
	renderer_context*,
	res_effect_ptr,
	enum_render_stage_type
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6417b0]
	return 0;
}

} // namespace render
} // namespace vostok
