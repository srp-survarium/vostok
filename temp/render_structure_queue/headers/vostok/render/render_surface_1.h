////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_SURFACE_H_INCLUDED
#define RENDER_RENDER_SURFACE_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;
class vostok::math::sphere;
struct vostok::render::render_geometry;
class vostok::render::enum_vertex_input_type;
typedef vostok::resources::resource_ptr<vostok::render::material_effects_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_effects_instance_ptr;
class vostok::render::material_effects_instance;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::memory::chunk_reader;
class vostok::render::batched_geometry_interface;
class vostok::render::material_effects;

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

class render_surface : public boost::noncopyable {
public:
	inline								render_surface			( ) { /* no source */ }
	virtual								~render_surface			( );

	inline	enum_vertex_input_type		get_vertex_input_type	( ) { /* no source */ }

	virtual	void						load					( configs::binary_config_value const& properties, memory::chunk_reader& chunk );

			void						set_material_effects	( material_effects_instance_ptr mtl_instance_ptr, pcstr material_name );
			void						set_default_material	( );

	virtual	bool						is_dynamic				( ) const = 0;

	virtual	void						fill_lpv_vertex_color	( batched_geometry_interface* arg_0, float4x4 const& arg_1 ) { /* no source */ }

	virtual	void						add_shadow_vertices		( batched_geometry_interface* arg_0, float4x4 const& arg_1 ) { /* no source */ }

			material_effects&			get_material_effects	( );

	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	enum_vertex_input_type				m_vertex_input_type;
	/* 0x0008 */	math::aabb							m_aabbox;
	/* 0x0020 */	math::sphere						m_bounding_sphere;
	/* 0x0030 */	render_geometry						m_render_geometry;
	/* 0x0094 */	material_effects_instance_ptr		m_materail_effects_instance;
	/* 0x0098 */	float								m_streaming_texture_factor;
}; // class render_surface

STATIC_SIZE_ASSERT(render_surface, 0x9C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_SURFACE_H_INCLUDED
