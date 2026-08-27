#include "pch.h"
#include "batched_geometry.h"
#include "batched_vertex_source.h"
#include "system_renderer.h"
#include <vostok/render/core/backend.h>
#include <vostok/render/core/dx11/decl_utils.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include "possible_sectors_holder.h"
#include <vostok/render/engine/model_format.h>
#include "render_model_static.h"
#include "streaming.h"

#pragma warning( push )
#pragma warning( disable : 4995 )
#include <d3dx9mesh.h>
#pragma warning( pop )

#pragma comment( lib, "d3dx9.lib" )

namespace vostok {
namespace render {

float lod_def_params[] = {
	20.f, 40.f, 240.f,
	.4f, .2f, .001f
};

static_render_surface::static_render_surface( bool colored )
{
	m_vertex_input_type	= colored ? static_mesh_vertex_colored_input_type : static_mesh_vertex_input_type;

}

template < u32 Size >
static bool read_diffuse_colors(
	material_effects_instance_ptr		m_materail_effects_instance,
	math::color (&results)[Size][Size]
)
{
	if (!m_materail_effects_instance)
		return false;

	res_effect_ptr& effect = m_materail_effects_instance->get_material_effects().m_effects[gbuffer_render_stage];

	if (!effect)
		return false;

	render_target_ptr rt = resource_manager::ref().create_render_target(
		"$user$diffuse_color",
		Size,
		Size,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		enum_rt_usage_render_target,
		res_texture_ptr(), 0, D3D11_USAGE_DEFAULT, 1, 0
	);
	res_texture_ptr tex = resource_manager::ref().create_texture( "$user$diffuse_color", NULL, 0, false, true, true, u32(-1) );

	effect->apply(3, 0);
	system_renderer::ref().fill_surface( rt, render_target_ptr(), render_target_ptr(), render_target_ptr(), render_target_ptr(), false, NULL, 0.f, 0.f, 1.f, 1.f );

	device::ref().d3d_context()->Flush();
	backend::ref().flush();

	res_texture_ptr lockable_texture = resource_manager::ref().create_texture2d(
		"$user$diffuse_color_lockable",
		Size,
		Size,
		NULL,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_USAGE_STAGING,
		1,
		1,
		false
	);

	resource_manager::ref().copy2D(
		lockable_texture.c_ptr(),
		0, 0,
		tex.c_ptr(), 0, 0,
		Size, Size, 0, 0
	);

	device::ref().d3d_context()->Flush();
	backend::ref().flush();

	u32 row_pitch;
	pbyte data = static_cast<pbyte>(
		lockable_texture->map2D(D3D11_MAP_READ, 0, row_pitch)
	);

	if (!data)
		return false;

	for (u32 row = 0; row < Size; ++row)
	{

		for (u32 column = 0; column < Size; ++column)
			results[column][row] = reinterpret_cast<math::color const*>(data)[column];

		data += row_pitch;
	}

	lockable_texture->unmap2D(0);

	return true;
}

template bool read_diffuse_colors< 64 >(
	material_effects_instance_ptr,
	math::color (&)[64][64]
);

static float frac( float f )
{
	return math::abs(f) - math::abs(math::floor(f));
}

template < u32 Size >
static math::color interpolated_color(
	math::color (&color_grid)[Size][Size],
	const float2 uv
)
{
	const float x = frac(uv.x) * Size;
	const float y = frac(uv.y) * Size;

	u32 grid_x = math::floor(x);
	math::clamp(grid_x, 0u, Size - 1);

	u32 grid_y = math::floor(y);
	math::clamp(grid_y, 0u, Size - 1);

	return color_grid[grid_x][grid_y];
}

template math::color interpolated_color< 64 >(
	math::color (&)[64][64],
	const float2
);

template < typename StaticVertex, typename OptimizedVertex >
static void create_shadow_pass_geometry_type(
	render_geometry&					in_render_geometry,
	pcbyte								data,
	const u32							num_vertices,
	const u32							stride,
	D3D11_INPUT_ELEMENT_DESC const*		layout,
	const u32							num_decl_elements
)
{
	OptimizedVertex* temp_data = ALLOC(OptimizedVertex, num_vertices);

	for (u32 i = 0; i < num_vertices; ++i)
		temp_data[i].set(*reinterpret_cast<StaticVertex const*>(data + i * sizeof(StaticVertex)));

	untyped_buffer_ptr vb = resource_manager::ref().create_buffer(
		num_vertices * sizeof(OptimizedVertex),
		temp_data,
		enum_buffer_type_vertex,
		false,
		false
	);

	untyped_buffer_ptr ib = in_render_geometry.geom->m_ib;
	res_declaration_ptr decl = resource_manager::ref().create_declaration(layout, num_decl_elements);
	in_render_geometry.shadow_pass_geom = resource_manager::ref().create_geometry(
		&*decl,
		sizeof(OptimizedVertex),
		*vb,
		*ib
	);

	FREE(temp_data);
}

template < typename StaticVertex >
static void fill_static_lpv_vertex_color(
	batched_geometry_interface*			in_out_lpv_geometry,
	render_geometry&					in_render_geometry,
	material_effects_instance_ptr&		in_materail_effects_instance,
	float4x4 const&						in_transform
)
{
	math::color color_grid[64][64];
	bool const diffuse_colors_read = read_diffuse_colors( in_materail_effects_instance, color_grid );
	if ( !diffuse_colors_read )
		return;

	untyped_buffer_ptr vb = in_render_geometry.geom->m_vb;
	u32 const num_vertices = vb->size( ) / in_render_geometry.geom->m_vb_stride;
	StaticVertex* temp_data = static_cast<StaticVertex*>( MALLOC( vb->size( ), "" ) );
	lpv_vertex* lpv_temp_data = ALLOC( lpv_vertex, num_vertices );
	batched_vertex_source* static_temp_data = ALLOC( batched_vertex_source, num_vertices );

	untyped_buffer_ptr temp_vb = resource_manager::ref( ).create_buffer(
		vb->size( ),
		temp_data,
		enum_buffer_type_vertex,
		false,
		true
	);
	device::ref( ).d3d_context( )->CopyResource(
		temp_vb->hardware_buffer( ),
		vb->hardware_buffer( )
	);
	device::ref( ).d3d_context( )->Flush( );
	StaticVertex const* data = static_cast<StaticVertex const*>( temp_vb->map( D3D11_MAP_READ ) );

	math::color value;
	for ( u32 i = 0; i < num_vertices; ++i ) {
		temp_data[i] = data[i];
		value = interpolated_color( color_grid, temp_data[i].uv );
		value.a = temp_data[i].tangent.a;

		lpv_temp_data[i].clr = value;
		lpv_temp_data[i].position = temp_data[i].position;
		lpv_temp_data[i].normal = temp_data[i].normal;

		static_temp_data[i].clr = value;
		static_temp_data[i].position = temp_data[i].position;
		static_temp_data[i].normal = temp_data[i].normal;
		static_temp_data[i].tangent = math::color( 0 );
		static_temp_data[i].binormal = math::color( 0 );
		static_temp_data[i].uv = temp_data[i].uv;
	}

	temp_vb->unmap( );

	u32 const num_indices = in_render_geometry.geom->m_ib->size( ) / sizeof( u16 );
	u16* indices_temp_data = static_cast<u16*>( MALLOC( in_render_geometry.geom->m_ib->size( ), "" ) );
	untyped_buffer_ptr temp_ib = resource_manager::ref( ).create_buffer(
		in_render_geometry.geom->m_ib->size( ),
		indices_temp_data,
		enum_buffer_type_index,
		false,
		true
	);
	device::ref( ).d3d_context( )->CopyResource(
		temp_ib->hardware_buffer( ),
		in_render_geometry.geom->m_ib->hardware_buffer( )
	);
	device::ref( ).d3d_context( )->Flush( );
	memory::copy(
		indices_temp_data,
		num_indices * sizeof( u16 ),
		temp_ib->map( D3D11_MAP_READ ),
		in_render_geometry.geom->m_ib->size( )
	);
	temp_ib->unmap( );

	if ( in_out_lpv_geometry )
		in_out_lpv_geometry->add_data(
			static_temp_data,
			num_vertices,
			indices_temp_data,
			num_indices,
			in_transform,
			material_effects_instance_ptr( )
		);

	untyped_buffer_ptr lpv_vb = resource_manager::ref( ).create_buffer(
		num_vertices * sizeof( lpv_vertex ),
		lpv_temp_data,
		enum_buffer_type_vertex,
		false,
		false
	);

	D3D11_INPUT_ELEMENT_DESC const lpv_layout[3] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	untyped_buffer_ptr ib = in_render_geometry.geom->m_ib;
	res_declaration_ptr decl = resource_manager::ref( ).create_declaration(
		lpv_layout,
		array_size( lpv_layout )
	);
	in_render_geometry.lpv_pass_geom = resource_manager::ref( ).create_geometry(
		decl.c_ptr( ),
		sizeof( lpv_vertex ),
		*lpv_vb,
		*ib
	);

	FREE( temp_data );
	FREE( lpv_temp_data );
	FREE( static_temp_data );
	FREE( indices_temp_data );
}

void static_render_surface::fill_lpv_vertex_color( batched_geometry_interface* in_out_geometry, float4x4 const& transform )
{
	struct static_vertex0 {
		float3			position;
		math::color	normal;
		math::color	tangent;
		math::color	binormal;
		float2			uv;
	};

	struct colored_static_vertex : public static_vertex0 {
		math::color	color_component;
	};

	if (m_vertex_input_type == static_mesh_vertex_input_type)
		fill_static_lpv_vertex_color< static_vertex0 >(
			in_out_geometry,
			m_render_geometry,
			m_materail_effects_instance,
			transform
		);
	else if (m_vertex_input_type == static_mesh_vertex_colored_input_type)
		fill_static_lpv_vertex_color< colored_static_vertex >(
			in_out_geometry,
			m_render_geometry,
			m_materail_effects_instance,
			transform
		);
}

template < typename StaticVertex >
static void fill_source_vertices_impl(
	render_geometry&					in_render_geometry,
	vector< batched_vertex_source >&	out_vertices,
	vector< u16 >&						out_indices
)
{
	untyped_buffer_ptr vb = in_render_geometry.geom->m_vb;
	u32 const num_vertices = vb->size( ) / in_render_geometry.geom->m_vb_stride;
	StaticVertex* temp_data = static_cast<StaticVertex*>( MALLOC( vb->size( ), "" ) );

	out_vertices.reserve( num_vertices );
	out_vertices.resize( num_vertices );

	untyped_buffer_ptr temp_vb = resource_manager::ref( ).create_buffer(
		vb->size( ),
		temp_data,
		enum_buffer_type_vertex,
		false,
		true
	);
	device::ref( ).d3d_context( )->CopyResource(
		temp_vb->hardware_buffer( ),
		vb->hardware_buffer( )
	);
	device::ref( ).d3d_context( )->Flush( );
	StaticVertex const* data = static_cast<StaticVertex const*>( temp_vb->map( D3D11_MAP_READ ) );

	for ( u32 i = 0; i < num_vertices; ++i ) {
		temp_data[i] = data[i];
		out_vertices[i].clr = math::color( 0 );
		out_vertices[i].position = temp_data[i].position;
		out_vertices[i].normal = temp_data[i].normal;
		out_vertices[i].tangent = math::color( 0 );
		out_vertices[i].binormal = math::color( 0 );
		out_vertices[i].uv = temp_data[i].uv;
	}

	temp_vb->unmap( );

	u32 const num_indices = in_render_geometry.geom->m_ib->size( ) / sizeof( u16 );
	u16* indices_temp_data = static_cast<u16*>( MALLOC( in_render_geometry.geom->m_ib->size( ), "" ) );
	out_indices.reserve( num_indices );
	out_indices.resize( num_indices );

	untyped_buffer_ptr temp_ib = resource_manager::ref( ).create_buffer(
		in_render_geometry.geom->m_ib->size( ),
		indices_temp_data,
		enum_buffer_type_index,
		false,
		true
	);
	device::ref( ).d3d_context( )->CopyResource(
		temp_ib->hardware_buffer( ),
		in_render_geometry.geom->m_ib->hardware_buffer( )
	);
	device::ref( ).d3d_context( )->Flush( );
	memory::copy(
		out_indices.begin( ),
		out_indices.size( ) * sizeof( u16 ),
		temp_ib->map( D3D11_MAP_READ ),
		in_render_geometry.geom->m_ib->size( )
	);
	temp_ib->unmap( );

	FREE( temp_data );
	FREE( indices_temp_data );
}

static void fill_source_vertices(
	const enum_vertex_input_type		in_vertex_input_type,
	render_geometry&					in_render_geometry,
	vector< batched_vertex_source >&	out_vertices,
	vector< u16 >&						out_indices
)
{
	struct static_vertex {
		float3			position;
		math::color	normal;
		math::color	tangent;
		math::color	binormal;
		float2			uv;
	};

	struct colored_static_vertex : public static_vertex {
		math::color	color_component;
	};

	if (in_vertex_input_type == static_mesh_vertex_input_type)
		fill_source_vertices_impl< static_vertex >(in_render_geometry, out_vertices, out_indices);
	else if (in_vertex_input_type == static_mesh_vertex_colored_input_type)
		fill_source_vertices_impl< colored_static_vertex >(in_render_geometry, out_vertices, out_indices);
}

void static_render_surface::add_shadow_vertices( batched_geometry_interface* in_out_geometry, float4x4 const& transform )
{
	vector< batched_vertex_source > out_vertices;
	vector< u16 > out_indices;

	fill_source_vertices(m_vertex_input_type, m_render_geometry, out_vertices, out_indices);

	if (!out_vertices.size() || !out_indices.size())
		return;

	in_out_geometry->add_data(
		out_vertices.begin(),
		out_vertices.size(),
		out_indices.begin(),
		out_indices.size(),
		transform,
		m_materail_effects_instance->get_material_effects().is_use_alpha_test ?
			m_materail_effects_instance :
			material_effects_instance_ptr()
	);
}

void static_render_surface::create_shadow_pass_geometry( pcbyte data, const u32 num_vertices, const u32 stride )
{
	struct static_vertex0 {
		float3			position;
		math::color	normal;
		math::color	tangent;
		math::color	binormal;
		float2			uv;
	};

	struct colored_static_vertex : public static_vertex0 {
		math::color	color_component;
	};

	struct opt_static_vertex {
		float3			position;
		math::color	normal;
		float2			uv;

		void set( static_vertex0 const& source )
		{
			position = source.position;
			normal = source.normal;
			uv = source.uv;
		}
	};

	struct colored_opt_static_vertex {
		float3			position;
		math::color	normal;
		float2			uv;
		math::color	color;

		void set( colored_static_vertex const& source )
		{
			position = source.position;
			normal = source.normal;
			uv = source.uv;
			color = source.color_component;
		}
	};

	const D3D11_INPUT_ELEMENT_DESC layout[3] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	const D3D11_INPUT_ELEMENT_DESC colored_layout[4] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (m_vertex_input_type == static_mesh_vertex_input_type)
		create_shadow_pass_geometry_type< static_vertex0, opt_static_vertex >(
			m_render_geometry,
			data,
			num_vertices,
			stride,
			layout,
			array_size(layout)
		);
	else if (m_vertex_input_type == static_mesh_vertex_colored_input_type)
		create_shadow_pass_geometry_type< colored_static_vertex, colored_opt_static_vertex >(
			m_render_geometry,
			data,
			num_vertices,
			stride,
			colored_layout,
			array_size(colored_layout)
		);
}

void static_render_surface::load( configs::binary_config_value const& properties, memory::chunk_reader& chunk )
{
	render_surface::load( properties, chunk );

	memory::reader reader = chunk.open_reader( model_chunk_vertices );
	D3DVERTEXELEMENT9 const* vFormat =
		reinterpret_cast<D3DVERTEXELEMENT9 const*>( reader.pointer( ) );
	u32 const dcl_len = D3DXGetDeclLength( vFormat ) + 1;

	vector<D3D11_INPUT_ELEMENT_DESC> decl_code;
	decl_utils::convert_vertex_declaration(
		vector<D3DVERTEXELEMENT9>( vFormat, vFormat + dcl_len ),
		decl_code
	);

	res_declaration_ptr decl = resource_manager::ref( ).create_declaration(
		decl_code.begin( ),
		decl_code.size( )
	);
	reader.advance( dcl_len * sizeof( D3DVERTEXELEMENT9 ) );

	m_render_geometry.vertex_count = reader.r_u32( );
	if ( m_render_geometry.vertex_count > 1024 * 64 )
	{
		LOG_ERROR( "vertex buffer size > 1024 * 64!" );
	}
	u32 vStride = D3DXGetDeclVertexSize( vFormat, 0 );
	untyped_buffer_ptr vb = resource_manager::ref( ).create_buffer(
		m_render_geometry.vertex_count * vStride,
		reader.pointer( ),
		enum_buffer_type_vertex,
		false,
		false
	);

	reader = chunk.open_reader( model_chunk_indices );
	m_render_geometry.index_count = reader.r_u32( );
	m_render_geometry.primitive_count = m_render_geometry.index_count / 3;
	untyped_buffer_ptr ib = resource_manager::ref( ).create_buffer(
		m_render_geometry.index_count * sizeof( u16 ),
		reader.pointer( ),
		enum_buffer_type_index,
		false,
		false
	);

	m_render_geometry.geom = resource_manager::ref( ).create_geometry(
		decl.c_ptr( ),
		vStride,
		*vb,
		*ib
	);

	reader = chunk.open_reader( model_chunk_vertices );
	reader.advance( dcl_len * sizeof( D3DVERTEXELEMENT9 ) + sizeof( u32 ) );
	create_shadow_pass_geometry(
		reader.pointer( ),
		m_render_geometry.vertex_count,
		vStride
	);

	reader = chunk.open_reader( model_chunk_vertices );
	reader.advance( dcl_len * sizeof( D3DVERTEXELEMENT9 ) + sizeof( u32 ) );
	struct static_vertex {
		float3 position;
		math::color normal;
		math::color tangent;
		math::color binormal;
		float2 uv;
	};
	static_vertex* vertices = (static_vertex*)reader.pointer( );
	reader = chunk.open_reader( model_chunk_indices );
	reader.advance( sizeof( u32 ) );
	m_streaming_texture_factor = calculate_streaming_texture_factor(
		&vertices->position,
		&vertices->uv,
		m_render_geometry.vertex_count,
		vStride,
		reinterpret_cast<u16 const*>( reader.pointer( ) ),
		m_render_geometry.index_count
	);
}

static_render_model_instance::static_render_model_instance( )
	: m_original			( NULL ),
	  m_instances_count	( 0 ),
	  m_surface_instances	( NULL )
{
}

static_render_model_instance::~static_render_model_instance( )
{
	DELETE			( m_sectors_holder );
	DELETE_ARRAY		( m_surface_instances );

}

void static_render_model_instance::assign_original( static_render_model_ptr v )
{
	m_original			= v;
	m_instances_count	= m_original->m_childs_count;
	m_surface_instances = NEW_ARRAY( render_surface_instance, m_instances_count );

	for( u8 i = 0; i < m_instances_count; ++i )
	{
		render_surface_instance& info = m_surface_instances[i];
		info.m_parent				= this;
		info.m_render_surface		= m_original->m_childs[i];
		info.m_transform			= &m_transform;
		info.m_flags				= visible_flag | cast_shadow_flag;
	}

}

void static_render_model_instance::set_lod_params(
	u8			type,
	bool		use_default,
	float		p0,
	float		p1,
	float		p2
)
{
	model_lods_descriptor* lods = m_original->m_lods_descriptor;
	lods->m_lod_calc_type = type;
	lods->m_lod_params_default = use_default;
	lods->m_lod_custom_params[0] = p0;
	lods->m_lod_custom_params[1] = p1;
	lods->m_lod_custom_params[2] = p2;
}

u8 static_render_model_instance::select_lod( float4x4 const& mat_vp, float3 const& view_pos )
{
	model_lods_descriptor* lods = m_original->m_lods_descriptor;

	if (!lods->m_lod_surfaces_count[0])
		return 0xaa;

	if (!lods->m_lod_surfaces_count[1])
		return 0;

	float* params = lods->m_lod_params_default ? lod_def_params + lods->m_lod_calc_type * 3 : lods->m_lod_custom_params;

	float params_mult = 1.f;
	if (!options::ref().current.m_geometry_quality)
		params_mult = .75f;

	math::aabb owner_aabb = m_original->m_aabbox;
	owner_aabb.modify(m_transform);

	switch (lods->m_lod_calc_type)
	{
	case 0:
		{
			float distance = (view_pos - owner_aabb.center()).length();

			if (distance < params[0] * params_mult)
				return 0;

			if (distance < params[1] * params_mult)
				return 1;

			if (distance < params[2] * params_mult)
				return 2;

			return 3;
		}

	case 1:
		{
			float3 vertices[8];
			owner_aabb.vertices(vertices);

			float3 pt_max(-1.f, -1.f, -1.f);
			float3 pt_min(1.f, 1.f, 1.f);

			for (u32 i = 0; i < 8; ++i)
			{
				float3 vertex = mat_vp.transform(vertices[i]);
				pt_min.min(vertex);
				pt_max.max(vertex);
			}

			float distance = math::max(pt_max.x - pt_min.x, pt_max.y - pt_min.y) * .5f;

			if (distance < params[2] * params_mult)
				return 3;

			if (distance < params[1] * params_mult)
				return 2;

			if (distance < params[0] * params_mult)
				return 1;

			return 0;
		}
	}

	return 0;
}

u32 static_render_model_instance::get_surfaces_count( u32 lod_id ) const
{
	return m_original->m_lods_descriptor->m_lod_surfaces_count[lod_id];
}

void static_render_model_instance::get_surfaces(
	float4x4 const*							mat_vp,
	float3 const*							view_pos,
	vector< render_surface_instance* >&		list,
	bool									visible_only,
	u8										lod_id,
	u32										surface_flags
)
{
	if (lod_id == u8(-1))

		lod_id = select_lod(*mat_vp, *view_pos);

	if (lod_id != 0xaa)
	{
		while (!m_original->m_lods_descriptor->m_lod_surfaces_count[lod_id]) {
			if (!lod_id)
				break;
			--lod_id;
		}
	}
	if (lod_id == 0xaa)
	{
		m_current_lod_index = 0;

		list.reserve(
			list.size() + m_instances_count);

		for (u8 i = 0; i < m_instances_count; ++i) {
			render_surface_instance* inst = m_surface_instances + i;
			if (!visible_only || inst->m_flags & surface_flags)
				list.push_back(inst);
		}

		return;
	}
	if (lod_id == 3)
		return;

	u8 lod_surfaces_count = m_original->m_lods_descriptor->m_lod_surfaces_count[lod_id];
	if (!lod_surfaces_count)
		return;

	list.reserve(list.size() + lod_surfaces_count);

	pbyte lod_surfaces = m_original->m_lods_descriptor->m_lod_surfaces[lod_id];

	bool need_reset_occlusion = false;

	if (m_current_lod_index != lod_id)

		need_reset_occlusion = true;
	m_current_lod_index = lod_id;

	for (; lod_surfaces_count; ++lod_surfaces, --lod_surfaces_count) {
		render_surface_instance* inst = m_surface_instances + *lod_surfaces;

		if (!visible_only ||
			inst->m_flags
			& visible_flag)
		{
			if (need_reset_occlusion)
			{
				inst->m_occlusion_info_index = u32(-1);
				inst->m_occluded = false;
			}
			list.push_back(inst);
		}
	}
}

bool static_render_model_instance::get_locator( pcstr locator_name, model_locator_item& result ) const
{

	return m_original->get_locator( locator_name, result );

}

void static_render_model_instance::get_surface_stats( u32 surface_id, surface_stats& stats ) const
{
	R_ASSERT			(surface_id<m_instances_count);
	render_surface_instance* inst = m_surface_instances+surface_id;
	stats.vcount		= inst->m_render_surface->m_render_geometry.vertex_count;
	stats.tricount		= inst->m_render_surface->m_render_geometry.primitive_count;
	if(inst->m_render_surface->m_materail_effects_instance.c_ptr())
		stats.material		= inst->m_render_surface->m_materail_effects_instance->get_material_name().c_str();
	else
		stats.material		= "_not_assigned";

}

void static_render_model_instance::add_sectors_holder( configs::binary_config_value sectotrs_cfg )
{
	m_sectors_holder = NEW( culling::possible_sectors_holder )( sectotrs_cfg );
}

} // namespace render
} // namespace vostok
