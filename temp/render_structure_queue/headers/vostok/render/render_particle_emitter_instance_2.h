////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_PARTICLE_EMITTER_INSTANCE_H_INCLUDED
#define RENDER_RENDER_PARTICLE_EMITTER_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;
class vostok::particle::particle_emitter_instance;
class vostok::uninitialized_reference<vostok::render::index_buffer>;
class vostok::uninitialized_reference<vostok::render::vertex_buffer>;
struct vostok::particle::beamtrail_parameters;
struct vostok::particle::billboard_parameters;
struct vostok::particle::render_particle_emitter_instance;
struct vostok::particle::world;
struct vostok::render::material_effects;
class vostok::particle::enum_particle_data_type;
class vostok::particle::enum_particle_locked_axis;
class vostok::particle::enum_particle_render_mode;
class vostok::particle::enum_particle_screen_alignment;
class vostok::particle::enum_particle_vertex_type;
class vostok::render::enum_vertex_input_type;
class vostok::resources::unmanaged_resource;
typedef vostok::intrusive_list<vostok::particle::base_particle,vostok::particle::base_particle *,128,vostok::threading::mutex,vostok::size_policy,vostok::no_debug_policy>
	vostok::particle::base_particle_list;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::resources::resource_ptr<vostok::render::material_effects_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_effects_instance_ptr;
class vostok::particle::base_particle;
class vostok::render::material_effects_instance;
class vostok::render::res_geometry;

namespace vostok {
namespace render {

enum vostok::particle::enum_particle_locked_axis
{
	particle_locked_axis_x				= 0x0,
	particle_locked_axis_y				= 0x1,
	particle_locked_axis_z				= 0x2,
	particle_locked_axis_negative_x		= 0x3,
	particle_locked_axis_negative_y		= 0x4,
	particle_locked_axis_negative_z		= 0x5,
	particle_locked_axis_rotate_x		= 0x6,
	particle_locked_axis_rotate_y		= 0x7,
	particle_locked_axis_rotate_z		= 0x8,
	particle_locked_axis_none			= 0x9,
};
enum vostok::particle::enum_particle_screen_alignment
{
	particle_screen_alignment_square		= 0x0,
	particle_screen_alignment_rectangle		= 0x1,
	particle_screen_alignment_to_path		= 0x2,
	particle_screen_alignment_to_axis		= 0x3,
};
enum vostok::particle::enum_particle_vertex_type
{
	particle_vertex_type_billboard		= 0x0,
	particle_vertex_type_billboard_subuv	= 0x1,
	particle_vertex_type_trail			= 0x2,
	particle_vertex_type_beam			= 0x3,
	particle_vertex_type_decal			= 0x4,
	particle_vertex_type_unknown		= 0x5,
};

class render_particle_emitter_instance : public particle::render_particle_emitter_instance {
public:
													render_particle_emitter_instance	(
														particle::world&						particle_world,
														particle::particle_emitter_instance&	particle_emitter_instance,
														particle::base_particle_list const&		particle_list,
														particle::billboard_parameters*			billboard_parameters,
														particle::beamtrail_parameters*			beamtrail_parameters,
														particle::enum_particle_locked_axis		locked_axis,
														particle::enum_particle_screen_alignment	screen_alignment,
														float4x4 const&							transform,
														float4 const&							instance_color
													);
	virtual											~render_particle_emitter_instance	( );

	inline	void									check_resize_render_buffers			( ) { /* no source */ }

			void									render_sprites						( );
			void									render_subuv_sprites				( );
			void									render_trails						(
														float3 const&				view_location,
														particle::base_particle*	start_particle,
														u32							num_particles
													);
			void									render_beams						( float3 const& view_location, u32 num_particles );
	inline	void									render_meshes						( ) { /* no source */ }

			void									render								( float3 const& view_location, const u32 num_particles );

			void									draw_debug							( float4x4 const& view_matrix, particle::enum_particle_render_mode debug_mode );

			enum_vertex_input_type					get_vertex_input_type				( );

	virtual	void									update_render_buffers				(
														particle::enum_particle_data_type		datatype,
														bool									use_subuv,
														u32										in_num_max_particles,
														u32										beamtrail_parameters_num_sheets
													) override;

	virtual	void									change_material						( resources::unmanaged_resource_ptr const& material ) override;

	virtual	void									set_transform						( float4x4 const& transform ) override;
	virtual	void									set_aabb							( math::aabb const& bbox ) override;

	virtual	bool									is_occluded							( ) const override;

	inline	particle::enum_particle_locked_axis		locked_axis							( ) const { /* no source */ }

	inline	particle::enum_particle_screen_alignment	screen_alignment					( ) const { /* no source */ }

	inline	float4x4 const&							transform							( ) const { /* no source */ }

	inline	particle::world&						particle_world						( ) const { /* no source */ }

			u32										get_num_particles					( ) const;

	inline	bool									is_sprite_based_geometry			( ) const { /* no source */ }

	inline	particle::enum_particle_vertex_type		vertex_type							( ) const { /* no source */ }

	inline	pcstr									material_name						( ) const { /* no source */ }

	inline	particle::particle_emitter_instance&	particle_emitter_instance			( ) const { /* no source */ }

			material_effects&						get_material_effects				( );
			math::aabb const&						get_aabb							( ) const;

	/* 0x0000 */	/* particle::render_particle_emitter_instance */
private:
	/* 0x0004 */	material_effects						m_material_effects;
	/* 0x0394 */	material_effects_instance_ptr			m_material_effects_ptr;
	/* 0x0398 */	math::aabb								m_bbox;
	/* 0x03b0 */	float4x4								m_transform;
	/* 0x03f0 */	res_geometry_ptr						m_particle_sprite_geometry;
	/* 0x03f4 */	res_geometry_ptr						m_subuv_particle_sprite_geometry;
	/* 0x03f8 */	res_geometry_ptr						m_particle_beamtrail_geometry;
	/* 0x0400 */	uninitialized_reference< vertex_buffer >	m_vertices;
	/* 0x0428 */	uninitialized_reference< index_buffer >	m_indices;
	/* 0x0448 */	particle::particle_emitter_instance&	m_particle_emitter_instance;
	/* 0x044c */	particle::base_particle_list const&		m_particle_list;
	/* 0x0450 */	particle::world&						m_particle_world;
	/* 0x0454 */	particle::billboard_parameters*			m_billboard_parameters;
	/* 0x0458 */	particle::beamtrail_parameters*			m_beamtrail_parameters;
	/* 0x045c */	float4 const&							m_instance_color;
	/* 0x0460 */	particle::enum_particle_vertex_type		m_vertex_type;
	/* 0x0464 */	particle::enum_particle_locked_axis		m_locked_axis;
	/* 0x0468 */	particle::enum_particle_screen_alignment	m_screen_alignment;
	/* 0x046c */	u32										m_num_vertices;
	/* 0x0470 */	u32										m_num_indices;
	/* 0x0474 */	u32										m_max_particles;
	/* 0x0478 */	u32										m_occlusion_info_index;
	/* 0x047c */	bool									m_occluded;
}; // class render_particle_emitter_instance

STATIC_SIZE_ASSERT(render_particle_emitter_instance, 0x480);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_PARTICLE_EMITTER_INSTANCE_H_INCLUDED
