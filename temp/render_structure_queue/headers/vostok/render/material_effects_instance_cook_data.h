////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MATERIAL_EFFECTS_INSTANCE_COOK_DATA_H_INCLUDED
#define RENDER_MATERIAL_EFFECTS_INSTANCE_COOK_DATA_H_INCLUDED

/* INCLUDES */
class vostok::render::enum_cull_mode;
class vostok::render::enum_vertex_input_type;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

enum vostok::render::enum_vertex_input_type
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
enum vostok::render::enum_cull_mode
{
	cull_mode_none		= 0x0,
	cull_mode_front		= 0x1,
	cull_mode_back		= 0x2,
};

struct material_effects_instance_cook_data {
	inline		~material_effects_instance_cook_data( ) { /* no source */ }
	inline		material_effects_instance_cook_data	(
					enum_vertex_input_type					in_vertex_input_type,
					resources::unmanaged_resource_ptr		in_material,
					bool									in_delete_in_cook,
					enum_cull_mode							in_cull_mode
				) { /* no source */ }

	/* 0x0000 */	enum_vertex_input_type					vertex_input_type;
	/* 0x0004 */	resources::unmanaged_resource_ptr		material;
	/* 0x0008 */	enum_cull_mode							cull_mode;
	/* 0x000c */	bool									delete_in_cook;
}; // struct material_effects_instance_cook_data

STATIC_SIZE_ASSERT(material_effects_instance_cook_data, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MATERIAL_EFFECTS_INSTANCE_COOK_DATA_H_INCLUDED
