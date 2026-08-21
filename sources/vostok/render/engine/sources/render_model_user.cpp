#include "pch.h"
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/untyped_buffer.h>
#include <vostok/render/engine/model_format.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include "material_effects_instance.h"
#include "render_model_user.h"

namespace vostok {
namespace render {

const D3D_INPUT_ELEMENT_DESC layout_editable[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,  D3D_INPUT_PER_VERTEX_DATA, 0 }, // pos.xyz
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 12, D3D_INPUT_PER_VERTEX_DATA, 0 },
};
struct vertex_editable
{
	float3 position;
	float2 uv;
};

const D3D_INPUT_ELEMENT_DESC layout_wire[] =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,  D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24, D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36, D3D_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 48, D3D_INPUT_PER_VERTEX_DATA, 0 },
};

struct vertex_wire {
	float3	position;
	float3	normal;
	float3	tangent;
	float3	binormal;
	float2	uv;
};

STATIC_SIZE_ASSERT( vertex_wire, 0x38 );

void user_render_surface::material_ready(
	resources::queries_result&		data,
	material_effects_instance_cook_data*	cook_data,
	char*							material_name
)
{
	// <0x632c94>|0x004|+0x012:'30'
	// <0>
	// <0x632ca6>|0x016|+0x013:'32'
	// <0x632cb9>|0x029|+0x05c:'33'
	// <0>
	// <0x632d15>|0x085|+0x019:'35'
	DELETE						( cook_data );

	if (data.is_successful())
		set_material_effects	( static_cast_resource_ptr<material_effects_instance_ptr>(data[0].get_unmanaged_resource()), material_name);

	FREE						( material_name );
}

void user_render_model_instance::assign_surface( user_render_surface* surface )
{
	// <0x632c10>|0x000|+0x006:'40'
	// <0x632c16>|0x006|+0x006:'41'
	// <0x632c1c>|0x00c|+0x00c:'42'
	// <0x632c28>|0x018|+0x006:'43'
	// <0x632c2e>|0x01e|+0x00a:'44'
	m_surface							= surface;
	m_surface_instance.m_render_surface = surface;
	m_surface_instance.m_transform		= &m_transform;
	m_surface_instance.m_parent			= this;
	m_surface_instance.m_flags			= visible_flag;
}

void user_render_model_instance::get_surfaces(
	float4x4 const*							mat_vp,
	float3 const*							view_pos,
	vector< render_surface_instance* >&		dest,
	bool									visible_only,
	u8										lod_id,
	u32										surface_flags
)
{
	// <0>
	// <0x632c52>|0x002|+0x021:'50'
	// <0x632c73>|0x023|-0x004:'50'
	// <0x632c6f>|0x01f|+0x014:'51'
	VOSTOK_UNREFERENCED_PARAMETERS	( mat_vp, view_pos, visible_only, lod_id, surface_flags );
	dest.push_back				( &m_surface_instance );
}

void user_render_model_instance::get_surface_stats( u32 surface_id, surface_stats& stats ) const
{

	VOSTOK_UNREFERENCED_PARAMETER	( stats );
	R_ASSERT		( surface_id==0 );
	NOT_IMPLEMENTED();
}

void user_render_surface_editable::load_from_chunk_data( memory::chunk_reader& chunk )
{
	// LOCALS
	// untyped_buffer_ptr 				vb
	// material_effects_instance_cook_data* cook_data
	// variant< 32 > 					user_data
	// res_declaration_ptr 				decl
	// fixed_string< 256 > 				material_name
	// untyped_buffer_ptr 				ib

	// <0>
	// <0x63403c>|0x00c|+0x00a:'78'
	// <0>
	// <0x634046>|0x016|+0x06f:'80'
	// <0>
	// <0x6340b5>|0x085|+0x06e:'82'
	// <0>
	// <0x634123>|0x0f3|+0x012:'84'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x634135>|0x105|+0x002:'90'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x634137>|0x107|+0x032:'96'
	// <0>
	// <1>
	// <2>
	// <0x634169>|0x139|+0x012:'100'
	// <0x63417b>|0x14b|+0x002:'101'
	// <0x63417d>|0x14d|+0x007:'102'
	// <0>
	// <0x634184>|0x154|+0x031:'104'
	// <0>
	// <0x6341b5>|0x185|+0x024:'106'
	// <0x6341d9>|0x1a9|+0x03e:'107'
	// <0>
	// <0x634217>|0x1e7|+0x006:'109'
	// <0>
	// <1>
	// <0x63421d>|0x1ed|+0x038:'112'
	// <0>
	// <0x634255>|0x225|+0x00a:'114'
	// <0x63425f>|0x22f|+0x005:'115'
	// <0>
	// <0x634264>|0x234|+0x028:'117'
	// <0x63428c>|0x25c|+0x00f:'118'
	// <0x63429b>|0x26b|+0x016:'119'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x6342b1>|0x281|+0x0c2:'133'
	m_vertex_input_type			= render::null_vertex_input_type;
	m_aabbox.min				= float3(-10.0f, -10.0f, -10.0f);
	m_aabbox.max				= float3(10.0f, 10.0f, 10.0f);
	memory::reader reader		= chunk.open_reader( model_texture);

	fixed_string<256> material_name	= reader.r_string();

	reader						= chunk.open_reader( model_chunk_vertices);

	// The visuals are create with dynamic usage temporary.
	bool dynamic		= true;

	m_render_geometry.vertex_count		= reader.r_u32				();
	u32 stride							= sizeof(vertex_editable);
	untyped_buffer_ptr vb				= resource_manager::ref().create_buffer(
		m_render_geometry.vertex_count*stride,
		reader.pointer(),
		enum_buffer_type_vertex,
		dynamic,
		false);

//--- read indices here

	reader								= chunk.open_reader( model_chunk_indices);
	m_render_geometry.index_count		= reader.r_u32();
	m_render_geometry.primitive_count	= m_render_geometry.index_count/3;

	untyped_buffer_ptr ib				= resource_manager::ref().create_buffer( m_render_geometry.index_count*2, reader.pointer(), enum_buffer_type_index, false, false);

	ref_declaration decl				= resource_manager::ref().create_declaration( layout_editable, array_size( layout_editable ) );
	m_render_geometry.geom				= resource_manager::ref().create_geometry( &*decl, stride, *vb, *ib);

	m_vb								= &*vb;

	render::material_effects_instance_cook_data* cook_data =
		NEW(render::material_effects_instance_cook_data)(render::null_vertex_input_type, NULL, false);

	resources::user_data_variant		user_data;
	user_data.set						(cook_data);

	pstr c_material_name				= ALLOC(char, 256);
	vostok::memory::zero					(c_material_name, 256);
	vostok::strings::copy					(c_material_name, 256, material_name.c_str());

	resources::query_resource			(
		material_name.c_str(),
		resources::material_effects_instance_class,
		boost::bind(
			&user_render_surface_editable::material_ready,
			this,
			_1,
			cook_data,
			c_material_name
		),
		g_allocator,
		&user_data
	);
}

void user_render_surface_wire::load_from_chunk_data( memory::chunk_reader& chunk )
{
	// LOCALS
	// memory::reader 					reader
	// untyped_buffer_ptr 				vb
	// material_effects_instance_cook_data* cook_data
	// variant< 32 > 					user_data
	// u16* 							indices
	// res_declaration_ptr 				decl
	// const u16[18] 					faces_indices
	// fixed_string< 256 > 				material_name
	// untyped_buffer_ptr 				ib
	// const u32 						num_indices
	// float 							u_coord
	// float3 							up_vector
	// float3 							left_vector
	// float3 							source_pos
	// float3 							to_next
	// float3 							direction
	// u32 								vertex_index

	// <0>
	// <1>
	// <0x632d5d>|0x01d|+0x021:'158'
	// <0>
	// <0x632d7e>|0x03e|+0x074:'160'
	// <0>
	// <0x632df2>|0x0b2|+0x003:'162'
	// <0>
	// <0x632df5>|0x0b5|+0x014:'164'
	// <0>
	// <0x632e09>|0x0c9|+0x010:'166'
	// <0>
	// <0x632e19>|0x0d9|+0x023:'168'
	// <0>
	// <0x632e3c>|0x0fc|+0x00e:'170'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x632e4a>|0x10a|+0x015:'175'
	// <0x632e5f>|0x11f|+0x020:'176'
	// <0x632e7f>|0x13f|+0x01e:'177'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x632e9d>|0x15d|+0x016:'182'
	// <0x632eb3>|0x173|+0x01e:'183'
	// <0>
	// <1>
	// <2>
	// <0x632ed1>|0x191|+0x0c1:'187'
	// <0x632f92>|0x252|-0x0be:'187'
	// <0>
	// <0x632ed4>|0x194|+0x0c4:'189'
	// <0>
	// <0x632f98>|0x258|+0x008:'191'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x632fa0>|0x260|+0x013:'196'
	// <0x632fb3>|0x273|+0x049:'197'
	// <0x632ffc>|0x2bc|+0x002:'198'
	// <0x632ffe>|0x2be|+0x04a:'199'
	// <0>
	// <0x633048>|0x308|+0x024:'201'
	// <0>
	// <1>
	// <2>
	// <0x63306c>|0x32c|+0x08a:'205'
	// <0x6330f6>|0x3b6|+0x0de:'206'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6331d4>|0x494|-0x2eb:'212'
	// <0x632ee9>|0x1a9|+0x382:'213'
	// <0x63326b>|0x52b|-0x326:'213'
	// <0x632f45>|0x205|+0x3d3:'214'
	// <0x633318>|0x5d8|+0x0d9:'214'
	// <0>
	// <0x6333f1>|0x6b1|+0x0f8:'216'
	// <0x6334e9>|0x7a9|+0x0f2:'217'
	// <0x6335db>|0x89b|+0x0f2:'218'
	// <0>
	// <0x6336cd>|0x98d|+0x0b7:'220'
	// <0x633784>|0xa44|+0x0bf:'221'
	// <0x633843>|0xb03|+0x075:'222'
	// <0>
	// <0x6338b8>|0xb78|+0x0ec:'224'
	// <0x6339a4>|0xc64|+0x0eb:'225'
	// <0x633a8f>|0xd4f|+0x102:'226'
	// <0>
	// <0x633b91>|0xe51|-0xc34:'228'
	// <0>
	// <1>
	// <2>
	// <0x632f5d>|0x21d|+0xc37:'232'
	// <0x633b94>|0xe54|+0x04f:'232'
	// <0x633be3>|0xea3|-0xc7e:'233'
	// <0x632f65>|0x225|+0xc8c:'234'
	// <0x633bf1>|0xeb1|+0x005:'234'
	// <0>
	// <0x633bf6>|0xeb6|+0x094:'236'
	// <0>
	// <1>
	// <0x633c8a>|0xf4a|+0x034:'239'
	// <0x633cbe>|0xf7e|+0x042:'240'
	// <0x633d00>|0xfc0|+0x041:'241'
	// <0>
	// <0x633d41>|0x1001|+0x01d:'243'
	// <0x633d5e>|0x101e|+0x012:'244'
	// <0x633d70>|0x1030|+0x023:'245'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x633d93>|0x1053|+0x02c:'252'
	// <0>
	// <0x633dbf>|0x107f|+0x003:'254'
	// <0x633dc2>|0x1082|+0x007:'255'
	// <0>
	// <0x633dc9>|0x1089|+0x033:'257'
	// <0>
	// <0x633dfc>|0x10bc|+0x026:'259'
	// <0x633e22>|0x10e2|+0x036:'260'
	// <0>
	// <1>
	// <0x633e58>|0x1118|+0x02d:'263'
	// <0>
	// <0x633e85>|0x1145|+0x00c:'265'
	// <0x633e91>|0x1151|+0x01a:'266'
	// <0>
	// <0x633eab>|0x116b|+0x020:'268'
	// <0x633ecb>|0x118b|+0x00f:'269'
	// <0x633eda>|0x119a|+0x015:'270'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x633eef>|0x11af|+0x0c5:'284'
	m_vertex_input_type					= render::wires_vertex_input_type;

	memory::reader reader				= chunk.open_reader( model_texture);

	fixed_string<256> material_name		= reader.r_string();

	float const wire_width				= reader.r_float();

	reader								= chunk.open_reader( model_chunk_vertices);

	m_aabbox.zero						();

	u32 const s_num_vertices			= reader.r_u32();
	u32 const num_vertices				= s_num_vertices * 3;
	u32 const num_indices				= (s_num_vertices - 1) * 6 * 3;

	m_render_geometry.vertex_count		= num_vertices;
	u32 stride							= sizeof(vertex_wire);

	float3*			s_vertices			= (float3*)ALLOCA(sizeof(float3) * s_num_vertices);
	vertex_wire*	vertices			= (vertex_wire*)ALLOCA(sizeof(vertex_wire) * num_vertices);
	u16*			indices				= (u16*)ALLOCA(sizeof(u16) * num_indices);

	vertex_wire*	b_vertices			= vertices;
	u16*			b_indices			= indices;

	for (u32 vertex_index = 0; vertex_index < s_num_vertices; vertex_index++)
		s_vertices[vertex_index]		= reader.r_float3();

	ASSERT_CMP							(s_num_vertices, !=, 1);

	float u_coord						= 0.0f;

	for (u32 vertex_index = 0; vertex_index < s_num_vertices; vertex_index++)
	{
		float3 const source_pos			= s_vertices[vertex_index];

		float3 direction				= float3(0.0f, 0.0f, 0.0f);
		float3 to_next					= float3(0.0f, 0.0f, 0.0f);

		if (vertex_index + 1 == s_num_vertices)
			to_next						= s_vertices[vertex_index] - s_vertices[vertex_index - 1];
		else
			to_next						= s_vertices[vertex_index + 1] - s_vertices[vertex_index];

		direction						= math::normalize_safe(to_next, float3(0.0f, 0.0f, 0.0f));

		float3 const help_vector		= float3(0.0f, 1.0f, 0.0f);

		float3 const left_vector		= math::normalize(math::cross_product(direction, help_vector));
		float3 const up_vector			= math::normalize(math::cross_product(left_vector, direction));

		vertex_wire& A0					= vertices[vertex_index * 3 + 0];
		vertex_wire& A1					= vertices[vertex_index * 3 + 1];
		vertex_wire& A2					= vertices[vertex_index * 3 + 2];

		A0.position						= source_pos + 0.5f * wire_width * math::normalize_safe( up_vector, float3(0.0f, 0.0f, 0.0f));
		A1.position						= source_pos + 0.5f * wire_width * math::normalize_safe( left_vector - up_vector, float3(0.0f, 0.0f, 0.0f));
		A2.position						= source_pos + 0.5f * wire_width * math::normalize_safe(-left_vector - up_vector, float3(0.0f, 0.0f, 0.0f));

		m_aabbox.modify					(A0.position);
		m_aabbox.modify					(A1.position);
		m_aabbox.modify					(A2.position);

		A0.normal						= math::normalize(A0.position - source_pos);
		A1.normal						= math::normalize(A1.position - source_pos);
		A2.normal						= math::normalize(A2.position - source_pos);

		A0.tangent						= math::normalize(math::cross_product(A0.normal, direction));
		A1.tangent						= math::normalize(math::cross_product(A1.normal, direction));
		A2.tangent						= math::normalize(math::cross_product(A2.normal, direction));

		A0.binormal						= direction;
		A1.binormal						= direction;
		A2.binormal						= direction;

		A0.uv							= float2(u_coord, 0.0f);
		A1.uv							= float2(u_coord, 0.5f);
		A2.uv							= float2(u_coord, 0.1f);

		u_coord							+= math::length(to_next);
	}

	u16 const faces_indices[]			= {0, 1, 3, 1, 4, 3,
										   0, 5, 2, 0, 3, 5,
										   2, 4, 1, 2, 5, 4};

	for (u32 vertex_index = 0; vertex_index < s_num_vertices - 1; vertex_index++)
		for (u32 face_index = 0; face_index < array_size(faces_indices); face_index++)
			*indices++						= static_cast_checked<u16>(vertex_index * 3) + faces_indices[face_index];

	untyped_buffer_ptr vb				= resource_manager::ref().create_buffer(
		num_vertices * stride,
		b_vertices,
		enum_buffer_type_vertex,
		true,
		false
	);

	m_render_geometry.index_count		= num_indices;
	m_render_geometry.primitive_count	= num_indices / 3;

	untyped_buffer_ptr ib				= resource_manager::ref().create_buffer( sizeof(u16) * num_indices, b_indices, enum_buffer_type_index, false, false);

	ref_declaration decl				= resource_manager::ref().create_declaration( layout_wire, array_size( layout_wire ) );
	m_render_geometry.geom				= resource_manager::ref().create_geometry( &*decl, stride, *vb, *ib);

	render::material_effects_instance_cook_data* cook_data =
		NEW(render::material_effects_instance_cook_data)(m_vertex_input_type, NULL, false);

	resources::user_data_variant		user_data;
	user_data.set						(cook_data);

	pstr c_material_name				= ALLOC(char, 256);
	vostok::memory::zero					(c_material_name, 256);
	vostok::strings::copy					(c_material_name, 256, material_name.c_str());

	resources::query_resource			(
		material_name.c_str(),
		resources::material_effects_instance_class,
		boost::bind(
			&user_render_surface_wire::material_ready,
			this,
			_1,
			cook_data,
			c_material_name
		),
		g_allocator,
		&user_data
	);
}

} // namespace render
} // namespace vostok
