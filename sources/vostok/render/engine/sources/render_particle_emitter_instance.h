#ifndef VOSTOK_RENDER_ENGINE_RENDER_PARTICLE_EMITTER_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_PARTICLE_EMITTER_INSTANCE_H_INCLUDED

#include <vostok/math_aabb.h>
#include <vostok/particle/base_particle.h>
#include <vostok/particle/render_particle_emitter_instance.h>
#include <vostok/render/core/index_buffer.h>
#include <vostok/render/core/vertex_buffer.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/uninitialized_reference.h>

#include "material_effects.h"
#include "material_effects_instance.h"
#include "res_geometry.h"

namespace vostok {

namespace particle {

class particle_emitter_instance;
struct world;

enum enum_particle_render_mode;

typedef particle_list_type base_particle_list;

enum enum_particle_vertex_type {
	particle_vertex_type_billboard		= 0x0,
	particle_vertex_type_billboard_subuv	= 0x1,
	particle_vertex_type_trail			= 0x2,
	particle_vertex_type_beam			= 0x3,
	particle_vertex_type_decal			= 0x4,
	particle_vertex_type_unknown			= 0x5,
};

} // namespace particle

namespace render {

class render_particle_emitter_instance :
	public particle::render_particle_emitter_instance
{
public:
	render_particle_emitter_instance(
		particle::world&					particle_world,
		particle::particle_emitter_instance&		particle_emitter_instance,
		particle::base_particle_list const&		particle_list,
		particle::billboard_parameters*			billboard_parameters,
		particle::beamtrail_parameters*			beamtrail_parameters,
		particle::enum_particle_locked_axis		locked_axis,
		particle::enum_particle_screen_alignment	screen_alignment,
		float4x4 const&					transform,
		float4 const&					instance_color
	);

	virtual ~render_particle_emitter_instance( );

	void check_resize_render_buffers( ) { }
	void render_sprites( );
	void render_subuv_sprites( );
	void render_trails(
		float3 const&			view_location,
		particle::base_particle*	start_particle,
		u32					num_particles
	);
	void render_beams( float3 const& view_location, u32 num_particles );
	void render_meshes( ) { }
	void render( float3 const& view_location, u32 num_particles );
	void draw_debug(
		float4x4 const&				view_matrix,
		particle::enum_particle_render_mode	debug_mode
	);

	enum_vertex_input_type get_vertex_input_type( );

	virtual void update_render_buffers(
		particle::enum_particle_data_type	data_type,
		bool					use_subuv,
		u32					max_num_particles,
		u32					beamtrail_parameters_num_sheets
	) override;

	virtual void change_material(
		resources::unmanaged_resource_ptr const& material
	) override;
	virtual void set_transform( float4x4 const& transform ) override;
	virtual void set_aabb( math::aabb const& bbox ) override;

	bool is_occluded( ) const;

	particle::enum_particle_locked_axis locked_axis( ) const
	{
		return m_locked_axis;
	}

	particle::enum_particle_screen_alignment screen_alignment( ) const
	{
		return m_screen_alignment;
	}

	float4x4 const& transform( ) const { return m_transform; }
	particle::world& particle_world( ) const { return m_particle_world; }
	u32 get_num_particles( ) const;

	bool is_sprite_based_geometry( ) const
	{
		return m_vertex_type == particle::particle_vertex_type_billboard ||
			m_vertex_type == particle::particle_vertex_type_billboard_subuv ||
			m_vertex_type == particle::particle_vertex_type_trail ||
			m_vertex_type == particle::particle_vertex_type_beam;
	}

	particle::enum_particle_vertex_type vertex_type( ) const
	{
		return m_vertex_type;
	}

	pcstr material_name( ) const
	{
		return m_material_effects_ptr.c_ptr( ) ?
			m_material_effects_ptr->get_material_name( ).c_str( ) : "";
	}

	particle::particle_emitter_instance& particle_emitter_instance( ) const
	{
		return m_particle_emitter_instance;
	}

	material_effects& get_material_effects( );
	math::aabb const& get_aabb( ) const;

private:
	material_effects					m_material_effects;
	material_effects_instance_ptr			m_material_effects_ptr;
	math::aabb						m_bbox;
	float4x4						m_transform;
	res_geometry_ptr					m_particle_sprite_geometry;
	res_geometry_ptr					m_subuv_particle_sprite_geometry;
	res_geometry_ptr					m_particle_beamtrail_geometry;
	uninitialized_reference< vertex_buffer >		m_vertices;
	uninitialized_reference< index_buffer >		m_indices;
	particle::particle_emitter_instance&		m_particle_emitter_instance;
	particle::base_particle_list const&		m_particle_list;
	particle::world&					m_particle_world;
	particle::billboard_parameters*			m_billboard_parameters;
	particle::beamtrail_parameters*			m_beamtrail_parameters;
	float4 const&						m_instance_color;
	particle::enum_particle_vertex_type		m_vertex_type;
	particle::enum_particle_locked_axis		m_locked_axis;
	particle::enum_particle_screen_alignment	m_screen_alignment;
	u32							m_num_vertices;
	u32							m_num_indices;
	u32							m_max_particles;
	u32							m_occlusion_info_index;
	bool						m_occluded;
};

STATIC_SIZE_ASSERT( render_particle_emitter_instance, 0x480 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_PARTICLE_EMITTER_INSTANCE_H_INCLUDED
