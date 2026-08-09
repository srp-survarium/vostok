#include "pch.h"
#include <vostok/render/facade/particles.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include "material.h"
#include "material_effects_instance.h"
#include "render_particle_emitter_instance.h"
#include "statistics.h"
#include "system_renderer.h"

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

D3D11_INPUT_ELEMENT_DESC const v_particle_sprite_fvf[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 3, DXGI_FORMAT_R32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 4, DXGI_FORMAT_R32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 5, DXGI_FORMAT_R32G32B32_FLOAT, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

D3D11_INPUT_ELEMENT_DESC const v_subuv_particle_sprite_fvf[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 3, DXGI_FORMAT_R32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 4, DXGI_FORMAT_R32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 5, DXGI_FORMAT_R32G32B32_FLOAT, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 6, DXGI_FORMAT_R32G32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 7, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

D3D11_INPUT_ELEMENT_DESC const v_particle_beamtrail_fvf[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

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
	// FUNCTION BODY[0x5fe370]
}

render_particle_emitter_instance::~render_particle_emitter_instance( )
{
	// FUNCTION BODY[0x5fe260]
	if ( m_vertices.initialized() )
		VOSTOK_DESTROY_REFERENCE( m_vertices );

	if ( m_indices.initialized() )
		VOSTOK_DESTROY_REFERENCE( m_indices );
}

bool render_particle_emitter_instance::is_occluded( ) const
{
	// FUNCTION BODY[0x5fc780]
	return options::ref( ).current.m_use_hiz_occlusion_culling && m_occluded;
}

math::aabb const& render_particle_emitter_instance::get_aabb( ) const
{
	// FUNCTION BODY[0x5fc680]
	return m_bbox;
}

void render_particle_emitter_instance::update_render_buffers(
	particle::enum_particle_data_type datatype,
	bool use_subuv,
	u32 in_num_max_particles,
	u32 beamtrail_parameters_num_sheets
)
{
	// FUNCTION BODY[0x5fe4a0]
	m_particle_sprite_geometry = 0;
	m_subuv_particle_sprite_geometry = 0;
	m_particle_beamtrail_geometry = 0;
	m_num_vertices = 0;
	m_num_indices = 0;

	m_max_particles = math::max<u32>( m_max_particles, in_num_max_particles );
	m_max_particles = math::min<u32>( m_max_particles, 2000 );

	switch ( datatype ) {
	case particle::particle_data_type_billboard: {
		if ( use_subuv ) {
			m_vertex_type = particle::particle_vertex_type_billboard_subuv;
			m_num_vertices = sizeof( subuv_particle_sprite_vertex ) * m_max_particles * 4;
			m_num_indices = sizeof( subuv_particle_sprite_vertex ) * m_max_particles * 6;
			if ( m_vertices.initialized( ) )
				VOSTOK_DESTROY_REFERENCE( m_vertices );
			VOSTOK_CONSTRUCT_REFERENCE( m_vertices, vertex_buffer )( m_num_vertices );

			if ( m_indices.initialized( ) )
				VOSTOK_DESTROY_REFERENCE( m_indices );
			VOSTOK_CONSTRUCT_REFERENCE( m_indices, index_buffer )( m_num_indices );

			m_subuv_particle_sprite_geometry = resource_manager::ref( ).create_geometry(
				v_subuv_particle_sprite_fvf,
				sizeof( subuv_particle_sprite_vertex ),
				m_vertices->buffer( ),
				m_indices->buffer( )
			);
			m_particle_sprite_geometry = 0;
			m_particle_beamtrail_geometry = 0;
		} else {
			m_vertex_type = particle::particle_vertex_type_billboard;
			m_num_vertices = sizeof( subuv_particle_sprite_vertex ) * m_max_particles * 4;
			m_num_indices = sizeof( subuv_particle_sprite_vertex ) * m_max_particles * 6;
			if ( m_vertices.initialized( ) )
				VOSTOK_DESTROY_REFERENCE( m_vertices );
			VOSTOK_CONSTRUCT_REFERENCE( m_vertices, vertex_buffer )( m_num_vertices );

			if ( m_indices.initialized( ) )
				VOSTOK_DESTROY_REFERENCE( m_indices );
			VOSTOK_CONSTRUCT_REFERENCE( m_indices, index_buffer )( m_num_indices );

			m_particle_sprite_geometry = resource_manager::ref( ).create_geometry(
				v_particle_sprite_fvf,
				sizeof( particle_sprite_vertex ),
				m_vertices->buffer( ),
				m_indices->buffer( )
			);
			m_subuv_particle_sprite_geometry = 0;
			m_particle_beamtrail_geometry = 0;
		}
		break;
	}

	case particle::particle_data_type_trail:
	case particle::particle_data_type_beam: {
		if ( datatype == particle::particle_data_type_trail )
			m_vertex_type = particle::particle_vertex_type_trail;
		else
			m_vertex_type = particle::particle_vertex_type_beam;

		u32 num_sheets = math::max<u32>( beamtrail_parameters_num_sheets, 1 );
		num_sheets = math::min<u32>( num_sheets, 10000 );

		m_num_vertices = sizeof( subuv_particle_sprite_vertex ) * m_max_particles * 2 * num_sheets;
		m_num_indices = sizeof( subuv_particle_sprite_vertex ) * m_max_particles * 6 * num_sheets;
		if ( m_vertices.initialized( ) )
			VOSTOK_DESTROY_REFERENCE( m_vertices );
		VOSTOK_CONSTRUCT_REFERENCE( m_vertices, vertex_buffer )( m_num_vertices );

		if ( m_indices.initialized( ) )
			VOSTOK_DESTROY_REFERENCE( m_indices );
		VOSTOK_CONSTRUCT_REFERENCE( m_indices, index_buffer )( m_num_indices );

		m_particle_beamtrail_geometry = resource_manager::ref( ).create_geometry(
			v_particle_beamtrail_fvf,
			sizeof( particle_beamtrail_vertex ),
			m_vertices->buffer( ),
			m_indices->buffer( )
		);
		m_particle_sprite_geometry = 0;
		m_subuv_particle_sprite_geometry = 0;
		break;
	}

	case particle::particle_data_type_decal: {
		m_vertex_type = particle::particle_vertex_type_decal;
		break;
	}

	default: {
		m_vertex_type = particle::particle_vertex_type_unknown;
		break;
	}
	}
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
	// FUNCTION BODY[0x5fdd10]
	// claude@NOTE: canonical particle_sprite_vertex::set gained the gravity component
	// (P->gravity passed); the legacy ancestor predates it.
	u32 num_particles = get_num_particles();

	if (!num_particles)
		return;

	u32 i_offset = 0, v_offset = 0;

	u32 const num_vertices_per_particle = 4;
	u32 const num_incides_per_particle = 6;

	u16*			 pib = m_indices->lock( num_particles * num_incides_per_particle, i_offset);

	particle_sprite_vertex* pv		= (particle_sprite_vertex*)m_vertices->lock( num_particles * num_vertices_per_particle, sizeof(particle_sprite_vertex), v_offset);

	u32 p = 0;
	vostok::particle::base_particle* P = m_particle_list.front();
	while (P)
	{
		u32 v					= p * 4;
		u16 _v					= static_cast_checked<u16>(v);

		*pib++					= _v+0;
		*pib++					= _v+3;
		*pib++					= _v+2;
		*pib++					= _v+0;
		*pib++					= _v+2;
		*pib++					= _v+1;

		float4 final_color		= float4(
			P->color.x * m_instance_color.x,
			P->color.y * m_instance_color.y,
			P->color.z * m_instance_color.z,
			P->color.w * m_instance_color.w
		);

		float2 size = float2(P->size.x, P->size.y);
		if (m_screen_alignment==particle::particle_screen_alignment_square)
			size.y = size.x;

		pv->set(P->position, final_color, float2(0.f, 0.f), size, P->rotation, P->gravity, P->old_position);	pv++;
		pv->set(P->position, final_color, float2(0.f, 1.f), size, P->rotation, P->gravity, P->old_position);	pv++;
		pv->set(P->position, final_color, float2(1.f, 1.f), size, P->rotation, P->gravity, P->old_position);	pv++;
		pv->set(P->position, final_color, float2(1.f, 0.f), size, P->rotation, P->gravity, P->old_position);	pv++;

		p++;
		P = m_particle_list.get_next_of_object(P);
	}

	m_vertices->unlock();
	m_indices->unlock();

	m_particle_sprite_geometry->apply();
	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, num_incides_per_particle * num_particles, i_offset, v_offset);
	statistics::ref().visibility_stat_group.num_triangles.value += num_incides_per_particle * num_particles / 3;
}

static float frac( float f )
{
	return		( math::abs( f ) - math::abs( floorf( f ) ) );
}

void render_particle_emitter_instance::render_subuv_sprites( )
{
	// FUNCTION BODY[0x5fd360]
	// claude@NOTE: canonical subuv_particle_sprite_vertex::set gained the gravity
	// component (P->gravity passed); the legacy ancestor predates it.
	u32 num_particles = get_num_particles();

	if (!num_particles)
		return;

	u32 i_offset = 0, v_offset = 0;

	u32 const num_vertices_per_particle = 4;
	u32 const num_incides_per_particle = 6;

	u16*			 pib = m_indices->lock( num_particles * num_incides_per_particle, i_offset);

	subuv_particle_sprite_vertex* pv	= (subuv_particle_sprite_vertex*)m_vertices->lock( num_particles * num_vertices_per_particle, sizeof(subuv_particle_sprite_vertex), v_offset);

	u32 p = 0;
	vostok::particle::base_particle* P = m_particle_list.front();
	while (P)
	{
		float subimage_index = P->subimage_index;//lifetime * m_billboard_parameters->sub_image_horizontal * m_billboard_parameters->sub_image_vertical * m_billboard_parameters->sub_image_change_speed;

		float next_subimage_index = P->next_subimage_index;//subimage_index;

		float posU = 0.0f;
		float posV = 0.0f;

		float next_posU = 0.0f;
		float next_posV = 0.0f;

		float sizeU = 1.0f;
		float sizeV = 1.0f;

		if (m_billboard_parameters->sub_image_horizontal && m_billboard_parameters->sub_image_horizontal)
		{
			posU = float(u32(subimage_index) % m_billboard_parameters->sub_image_horizontal);
			posV = float(u32(subimage_index) / m_billboard_parameters->sub_image_horizontal);

			next_posU = float(u32(next_subimage_index) % m_billboard_parameters->sub_image_horizontal);
			next_posV = float(u32(next_subimage_index) / m_billboard_parameters->sub_image_horizontal);

			sizeU = 1.0f / float(m_billboard_parameters->sub_image_horizontal);
			sizeV = 1.0f / float(m_billboard_parameters->sub_image_vertical);
		}

		float u_offset			= frac(subimage_index);

		if (m_billboard_parameters->subuv_method == particle::particle_subuv_method_random_smooth)
		{
			float const f_time = P->get_linear_lifetime() * m_billboard_parameters->sub_image_changes;
			float const i_time = float(math::floor(f_time));

			u_offset			= f_time - i_time;
		}

		u32 v					= p * 4;
		u16 _v					= static_cast_checked<u16>(v);

		*pib++					= _v+0;
		*pib++					= _v+3;
		*pib++					= _v+2;
		*pib++					= _v+0;
		*pib++					= _v+2;
		*pib++					= _v+1;

		float4 final_color		= float4(
			P->color.x * m_instance_color.x,
			P->color.y * m_instance_color.y,
			P->color.z * m_instance_color.z,
			P->color.w * m_instance_color.w
		);

		float2 size = float2(P->size.x, P->size.y);
		if (m_screen_alignment==particle::particle_screen_alignment_square)
			size.y = size.x;

		pv->set(P->position, final_color, float2(sizeU*(posU+0), sizeV*(posV+0)), size, P->rotation, P->gravity, P->old_position, float2(0,0), float4(u_offset, sizeU*(next_posU+0), sizeV*(next_posV+0), 0)); pv++;
		pv->set(P->position, final_color, float2(sizeU*(posU+0), sizeV*(posV+1)), size, P->rotation, P->gravity, P->old_position, float2(0,1), float4(u_offset, sizeU*(next_posU+0), sizeV*(next_posV+1), 0)); pv++;
		pv->set(P->position, final_color, float2(sizeU*(posU+1), sizeV*(posV+1)), size, P->rotation, P->gravity, P->old_position, float2(1,1), float4(u_offset, sizeU*(next_posU+1), sizeV*(next_posV+1), 0)); pv++;
		pv->set(P->position, final_color, float2(sizeU*(posU+1), sizeV*(posV+0)), size, P->rotation, P->gravity, P->old_position, float2(1,0), float4(u_offset, sizeU*(next_posU+1), sizeV*(next_posV+0), 0)); pv++;

		p++;
		P = m_particle_list.get_next_of_object(P);
	}

	m_vertices->unlock();
	m_indices->unlock();

	m_subuv_particle_sprite_geometry->apply();
	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, num_incides_per_particle * num_particles, i_offset, v_offset);
	statistics::ref().visibility_stat_group.num_triangles.value += num_incides_per_particle * num_particles / 3;
}

void render_particle_emitter_instance::render( float3 const& view_location, u32 num_particles )
{
	// FUNCTION BODY[0x5fe1f0]
	switch (m_vertex_type)
	{
		case particle::particle_vertex_type_billboard:
			{
				BEGIN_CPUGPU_TIMER(statistics::ref().particles_stat_group.sprites_execute_time);
				render_sprites();
				END_CPUGPU_TIMER;
			}
		break;

		case particle::particle_vertex_type_billboard_subuv:
			{
				BEGIN_CPUGPU_TIMER(statistics::ref().particles_stat_group.sprites_execute_time);
				render_subuv_sprites();
				END_CPUGPU_TIMER;
			}
		break;

		case particle::particle_vertex_type_trail:
			{
				BEGIN_CPUGPU_TIMER(statistics::ref().particles_stat_group.beamtrails_execute_time);
				render_trails(view_location, m_particle_list.front(), num_particles);
				END_CPUGPU_TIMER;
			}
		break;

		case particle::particle_vertex_type_beam:
			{
				BEGIN_CPUGPU_TIMER(statistics::ref().particles_stat_group.beamtrails_execute_time);
				render_beams(view_location, num_particles);
				END_CPUGPU_TIMER;
			}
		break;
	};
}

void render_particle_emitter_instance::render_beams( float3 const& view_location, u32 num_particles )
{
	// FUNCTION BODY[0x5fd300]
	u32 const num_particle_per_beam = num_particles / m_beamtrail_parameters->num_beams;

	for (u32 beam_index=0; beam_index<m_beamtrail_parameters->num_beams; beam_index++)
	{
		particle::base_particle* P = m_particle_list.front();
		u32 particle_index = 0;
		while (P)
		{
			if (particle_index == beam_index * num_particle_per_beam)
				break;

			particle_index++;
			P = m_particle_list.get_next_of_object(P);
		}
		render_trails(view_location, P, num_particle_per_beam);
	}
}

void render_particle_emitter_instance::render_trails(
	float3 const& view_location,
	particle::base_particle* start_particle,
	u32 num_particles
)
{
	// FUNCTION BODY[0x5fc820]
	if (!m_beamtrail_parameters)
		return;

	if (!num_particles)
		return;

	u32 const num_sheets					= vostok::math::max<u32>(m_beamtrail_parameters->num_sheets, 1);
	u32 const num_vertices_per_particle		= 2;
	u32 const num_indices_per_particle		= 6;
	u32 const texture_tiles					= vostok::math::max<u32>(m_beamtrail_parameters->num_texture_tiles, 1);
	u32 const num_quads						= num_particles - 1;

	u32 i_offset = 0,
		v_offset = 0;

	u16* pib						= m_indices->lock( num_sheets * num_indices_per_particle * num_particles, i_offset);
	particle_beamtrail_vertex* pv	= (particle_beamtrail_vertex*)m_vertices->lock( num_sheets * num_vertices_per_particle * num_particles, sizeof(particle_beamtrail_vertex), v_offset);

	float distance_from_first_to_last = 0.0f;

	vostok::particle::base_particle* P = start_particle;
	for (u32 p=0; p<num_quads; p++)
	{
		vostok::particle::base_particle* nextP = m_particle_list.get_next_of_object(P);
		float s = 1.0f;

		if (p==num_quads-1)
		{
			//s = (1.0f - nextP->lifetime);
		}
		distance_from_first_to_last += s * float3(nextP->position-P->position).squared_length();
		P = m_particle_list.get_next_of_object(P);
	}


	float current_angle = 0.0f;
	float const angle_incrase = math::pi / float(num_sheets);

	for (u32 s=0; s<num_sheets; s++)
	{
		float current_uv_start = 0.0f;

		vostok::particle::base_particle* P = start_particle;
		vostok::particle::base_particle* prevP = 0;
		float3 prev_particle_dir;

		for (u32 p=0; p<num_quads; p++)
		{
			vostok::particle::base_particle* nextP = m_particle_list.get_next_of_object(P);

			float s = 1.0f;

			if (p==num_quads-1)
			{
				//s = (1.0f - nextP->lifetime);
			}
			float const distance_from_current_to_next = s * float3(nextP->position-P->position).squared_length();

			float const scaleUV		= distance_from_current_to_next / distance_from_first_to_last;

			float3 view_dir  	 	= math::normalize_safe(view_location - P->position);
			float3 particle_dir 	= math::normalize_safe(nextP->position - P->position);

			if (p!=0)
			{
				particle_dir 		= math::normalize_safe(nextP->position - prevP->position);
			}

			float4 final_color		= float4(P->color.x * m_instance_color.x, P->color.y * m_instance_color.y, P->color.z * m_instance_color.z, P->color.w * m_instance_color.w);

			float const current_v_pos = current_uv_start * float(texture_tiles);

			float3	right_a			= math::normalize_safe(math::cross_product(view_dir, particle_dir));

			if (s!=0)
			{
				if (num_particles!=2 && p == num_particles - 2)
				{
					particle_dir = prev_particle_dir;
				}
				float4x4 rot_matrix		= create_rotation(particle_dir.normalize_safe(particle_dir), current_angle);
				right_a					= rot_matrix.transform_position(right_a);
			}

			pv->set(P->position + right_a * P->size.y, final_color, float2(0,current_v_pos)); pv++;
			pv->set(P->position - right_a * P->size.y, final_color, float2(1,current_v_pos)); pv++;

			if (num_particles==2 || p == num_particles - 2)
			{
				current_uv_start += scaleUV;
				pv->set(nextP->position + right_a * P->size.y, final_color, float2(0,current_uv_start * float(texture_tiles))); pv++;
				pv->set(nextP->position - right_a * P->size.y, final_color, float2(1,current_uv_start * float(texture_tiles))); pv++;
			}

			current_uv_start += scaleUV;
			prev_particle_dir = particle_dir;
			prevP = P;
			P = m_particle_list.get_next_of_object(P);
		}
		current_angle += angle_incrase;
	}

	for (u16 s=0; s<num_sheets; s++)
	{
		for (u32 p=0; p<num_quads; p++)
		{
			u16 v					= static_cast_checked<u16>(num_particles * num_vertices_per_particle * s + p * 2);

			*pib++					= v+0;
			*pib++					= v+1;
			*pib++					= v+3;
			*pib++					= v+0;
			*pib++					= v+3;
			*pib++					= v+2;
		}
	}

	m_vertices->unlock();
	m_indices->unlock();

	m_particle_beamtrail_geometry->apply();

	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, num_sheets * num_indices_per_particle * num_quads, i_offset, v_offset);
	statistics::ref().visibility_stat_group.num_triangles.value += num_sheets * num_indices_per_particle * num_quads / 3;
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
	// FUNCTION BODY[0x5fc690]
	// TODO: fix it!!!
	if (!m_material_effects_ptr.c_ptr())
	{
		return material::nomaterial_material( get_vertex_input_type() );
	}
	else
	{
		return m_material_effects_ptr->get_material_effects();
	}
}

void render_particle_emitter_instance::change_material(
	resources::unmanaged_resource_ptr const& material
)
{
	// FUNCTION BODY[0x5fc7a0]
	// TODO: particle decal!
	m_material_effects_ptr	= vostok::static_cast_resource_ptr<material_effects_instance_ptr>(material);
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
	float4x4 const& view_matrix,
	particle::enum_particle_render_mode debug_mode
)
{
	// FUNCTION BODY[0x5fe890]
	vostok::math::float4x4 camera_to_world;
	camera_to_world.try_invert(view_matrix);

	vostok::math::float3 up_vector	 = camera_to_world.transform_direction(float3(0,1000,0)).normalize();
	vostok::math::float3 right_vector  = camera_to_world.transform_direction(float3(1000,0,0)).normalize();

	vostok::math::float3 view_location = camera_to_world.c.xyz();

	switch (debug_mode)
	{
		case particle::dots_particle_render_mode:
		{
			particle::base_particle* P = m_particle_list.front();
			while (P)
			{
				system_renderer::ref().draw_3D_point(P->position, 3, vostok::math::color(1.0f,1.0f,0.0f,1.0f), false);
				P = m_particle_list.get_next_of_object(P);
			}
		}
		break;

		case particle::size_particle_render_mode:
		{
			particle::base_particle* P = m_particle_list.front();
			while (P)
			{
				vostok::math::float3 points[2];

				points[0] = vostok::math::float3(P->position - right_vector * P->size.x * 0.5f);
				points[1] = vostok::math::float3(P->position + right_vector * P->size.x * 0.5f);

				system_renderer::ref().draw_screen_lines(points, sizeof(points)/sizeof(points[0]), vostok::math::color(1.0f,1.0f,0.0f,1.0f), 1, 0xffffffff, false, false);

				points[0] = vostok::math::float3(P->position - up_vector * P->size.y * 0.5f);
				points[1] = vostok::math::float3(P->position + up_vector * P->size.y * 0.5f);

				system_renderer::ref().draw_screen_lines(points, sizeof(points)/sizeof(points[0]), vostok::math::color(1.0f,1.0f,0.0f,1.0f), 1, 0xffffffff, false, false);

				P = m_particle_list.get_next_of_object(P);
			}
		}
		break;
	}
}

} // namespace render
} // namespace vostok
