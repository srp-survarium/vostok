// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef RENDER_ENUM_VERTEX_INPUT_TYPE_H_INCLUDED
#define RENDER_ENUM_VERTEX_INPUT_TYPE_H_INCLUDED
namespace vostok {
namespace render {

enum enum_vertex_input_type
{
	null_vertex_input_type				= 0x00,
	static_mesh_vertex_input_type		= 0x01,
	static_mesh_vertex_colored_input_type	= 0x02,
	skeletal_4_bones_mesh_vertex_input_type	= 0x03,
	skeletal_3_bones_mesh_vertex_input_type	= 0x04,
	skeletal_2_bones_mesh_vertex_input_type	= 0x05,
	skeletal_1_bones_mesh_vertex_input_type	= 0x06,
	particle_vertex_input_type			= 0x07,
	particle_subuv_vertex_input_type	= 0x08,
	particle_beamtrail_vertex_input_type	= 0x09,
	decal_vertex_input_type				= 0x0a,
	grassmesh_vertex_input_type			= 0x0b,
	post_process_vertex_input_type		= 0x0c,
	wires_vertex_input_type				= 0x0d,
	user_vertex_input_type				= 0x0e,
	num_vertex_input_types				= 0x0f,
	unknown_vertex_input_type			= 0x10,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_VERTEX_INPUT_TYPE_H_INCLUDED
