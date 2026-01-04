////////////////////////////////////////////////////////////////////////////
//	Created 	: 01.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_FACADE_VERTEX_INPUT_TYPE_H_INCLUDED
#define VOSTOK_RENDER_FACADE_VERTEX_INPUT_TYPE_H_INCLUDED

namespace vostok {
namespace render {

enum enum_vertex_input_type
{
	null_vertex_input_type = 0,
	static_mesh_vertex_input_type,
	skeletal_mesh_vertex_input_type,
	skeletal_4_bones_mesh_vertex_input_type = 0x3,
	skeletal_3_bones_mesh_vertex_input_type = 0x4,
	skeletal_2_bones_mesh_vertex_input_type = 0x5,
	skeletal_1_bones_mesh_vertex_input_type = 0x6,
	particle_vertex_input_type,
	particle_subuv_vertex_input_type,
	particle_beamtrail_vertex_input_type,
	decal_vertex_input_type,
	grassmesh_vertex_input_type,
	post_process_vertex_input_type,
	wires_vertex_input_type,
	user_vertex_input_type 					= 0xE,

	num_vertex_input_types,

	unknown_vertex_input_type,

	// sushi@TODO: Removed
	lensflares_vertex_input_type,
	terrain_vertex_input_type,
	grassbillboard_vertex_input_type,
	speedtree_branch_vertex_input_type,
	speedtree_frond_vertex_input_type,
	speedtree_leafmesh_vertex_input_type,
	speedtree_leafcard_vertex_input_type,
	speedtree_billboard_vertex_input_type,

}; // enum enum_vertex_input_type

enum_vertex_input_type	string_to_vertex_input_type		(pcstr name);
pcstr					vertex_input_type_to_string		(enum_vertex_input_type type);

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_VERTEX_INPUT_TYPE_H_INCLUDED
