#include "pch.h"
#include "render_particle_emitter_instance.h"

namespace vostok {
namespace render {

#pragma pack( push, 1 )

struct particle_beamtrail_vertex {
	void set( float3 position_value, float4 color_value, float2 uv_value )
	{
		position[0] = position_value.x;
		position[1] = position_value.y;
		position[2] = position_value.z;
		color[0] = color_value.x;
		color[1] = color_value.y;
		color[2] = color_value.z;
		color[3] = color_value.w;
		uv[0] = uv_value.x;
		uv[1] = uv_value.y;
	}

	float	position[3];
	float	color[4];
	float	uv[2];
};

STATIC_SIZE_ASSERT( particle_beamtrail_vertex, 0x24 );

struct particle_sprite_vertex {
	void set(
		float3	position_value,
		float4	color_value,
		float2	uv_value,
		float2	size_value,
		float	rotation_value,
		float	gravity_value,
		float3	old_position_value
	);

	particle_sprite_vertex( ) { }

	float3	position;
	float4	color;
	float2	uv;
	float2	size;
	float	rotation;
	float	gravity;
	float3	old_position;
};

STATIC_SIZE_ASSERT( particle_sprite_vertex, 0x40 );

struct subuv_particle_sprite_vertex : public particle_sprite_vertex {
	void set(
		float3	position_value,
		float4	color_value,
		float2	uv_value,
		float2	size_value,
		float	rotation_value,
		float	gravity_value,
		float3	old_position_value,
		float2	size_uv_value,
		float4	blend_uv_value
	)
	{
		particle_sprite_vertex::set(
			position_value,
			color_value,
			uv_value,
			size_value,
			rotation_value,
			gravity_value,
			old_position_value
		);
		size_uv = size_uv_value;
		blend_uv = blend_uv_value;
	}

	subuv_particle_sprite_vertex( ) { }

	float2	size_uv;
	float4	blend_uv;
};

STATIC_SIZE_ASSERT( subuv_particle_sprite_vertex, 0x58 );

#pragma pack( pop )

void particle_sprite_vertex::set(
	float3	position_value,
	float4	color_value,
	float2	uv_value,
	float2	size_value,
	float	rotation_value,
	float	gravity_value,
	float3	old_position_value
)
{
	// FUNCTION BODY[0x88620]
	position = position_value;
	color = color_value;
	uv = uv_value;
	size = size_value;
	rotation = rotation_value;
	gravity = gravity_value;
	old_position = old_position_value;
}

render_particle_emitter_instance::render_particle_emitter_instance(
	particle::world&					particle_world,
	particle::particle_emitter_instance&		particle_emitter_instance,
	particle::base_particle_list const&		particle_list,
	particle::billboard_parameters*			billboard_parameters,
	particle::beamtrail_parameters*			beamtrail_parameters,
	particle::enum_particle_locked_axis		locked_axis,
	particle::enum_particle_screen_alignment	screen_alignment,
	float4x4 const&					transform,
	float4 const&					instance_color
) :
	m_material_effects( ),
	m_material_effects_ptr( ),
	m_bbox( math::create_zero_aabb( ) ),
	m_transform( transform ),
	m_particle_sprite_geometry( ),
	m_subuv_particle_sprite_geometry( ),
	m_particle_beamtrail_geometry( ),
	m_vertices( ),
	m_indices( ),
	m_particle_emitter_instance( particle_emitter_instance ),
	m_particle_list( particle_list ),
	m_particle_world( particle_world ),
	m_billboard_parameters( billboard_parameters ),
	m_beamtrail_parameters( beamtrail_parameters ),
	m_instance_color( instance_color ),
	m_vertex_type( particle::particle_vertex_type_unknown ),
	m_locked_axis( locked_axis ),
	m_screen_alignment( screen_alignment ),
	m_num_vertices( 0 ),
	m_num_indices( 0 ),
	m_max_particles( 0 ),
	m_occlusion_info_index( 0 ),
	m_occluded( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fe370]
}

render_particle_emitter_instance::~render_particle_emitter_instance( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fe260]
}

bool render_particle_emitter_instance::is_occluded( ) const
{
	// FUNCTION BODY[0x5fc780]
	return m_occluded;
}

math::aabb const& render_particle_emitter_instance::get_aabb( ) const
{
	// FUNCTION BODY[0x5fc680]
	return m_bbox;
}

void render_particle_emitter_instance::update_render_buffers(
	particle::enum_particle_data_type,
	bool,
	u32,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fe4a0]
}

u32 render_particle_emitter_instance::get_num_particles( ) const
{
	// FUNCTION BODY[0x5fc660]
	u32 result = 0;
	particle::base_particle* current = m_particle_list.front( );
	while ( current ) {
		++result;
		current = m_particle_list.get_next_of_object( current );
	}
	return result;
}

void render_particle_emitter_instance::render_sprites( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fdd10]
}

static float frac( float value )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fc700]
	VOSTOK_UNREFERENCED_PARAMETER( value );
	return 0.0f;
}

void render_particle_emitter_instance::render_subuv_sprites( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fd360]
}

void render_particle_emitter_instance::render( float3 const&, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fe1f0]
}

void render_particle_emitter_instance::render_beams( float3 const&, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fd300]
}

void render_particle_emitter_instance::render_trails(
	float3 const&,
	particle::base_particle*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fc820]
}

enum_vertex_input_type render_particle_emitter_instance::get_vertex_input_type( )
{
	// FUNCTION BODY[0x5fc620]
	switch ( m_vertex_type ) {
	case particle::particle_vertex_type_billboard:
		return particle_vertex_input_type;
	case particle::particle_vertex_type_billboard_subuv:
		return particle_subuv_vertex_input_type;
	case particle::particle_vertex_type_beam:
	case particle::particle_vertex_type_trail:
		return particle_beamtrail_vertex_input_type;
	default:
		return null_vertex_input_type;
	}
}

material_effects& render_particle_emitter_instance::get_material_effects( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fc690]
	return m_material_effects;
}

void render_particle_emitter_instance::change_material(
	resources::unmanaged_resource_ptr const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fc7a0]
}

void render_particle_emitter_instance::set_transform( float4x4 const& transform )
{
	// FUNCTION BODY[0x5fc600]
	m_transform = transform;
}

void render_particle_emitter_instance::set_aabb( math::aabb const& bbox )
{
	// FUNCTION BODY[0x5fc5d0]
	m_bbox = bbox;
}

void render_particle_emitter_instance::draw_debug(
	float4x4 const&,
	particle::enum_particle_render_mode
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5fe890]
}

} // namespace render
} // namespace vostok
