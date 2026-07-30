#ifndef VOSTOK_RENDER_ENGINE_SCENE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_H_INCLUDED

#include <vostok/particle/engine.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/engine/world_pc.h>
#include <vostok/render/facade/volume_fog_parameters.h>
#include "material_effects_instance.h"
#include "render_model_instance_impl.h"
#include "res_texture.h"
#include "speedtree_instance.h"
#include "tracer_model_instance.h"

namespace vostok {

namespace particle {

typedef particle_list_type base_particle_list;

} // namespace particle

namespace vfs {

class vfs_locked_iterator;

} // namespace vfs

namespace render {

class render_surface_instance;
class system_renderer;

struct grass_world;
struct scene_configuration;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

class scene : public base_scene {
public:
	struct particle_engine :
		public particle::engine,
		private boost::noncopyable
	{
		explicit particle_engine( scene* const in_scene ) :
			m_particle_world( 0 ),
			m_scene( in_scene )
		{
		}

		void set_particle_world( particle::world& particle_world )
		{
			m_particle_world = &particle_world;
		}

		virtual particle::render_particle_emitter_instance* create_render_emitter_instance(
			particle::particle_emitter_instance&	particle_emitter_instance,
			particle::base_particle_list const&		particle_list,
			particle::billboard_parameters*			billboard_parameters,
			particle::beamtrail_parameters*			beamtrail_parameters,
			particle::enum_particle_locked_axis		locked_axis,
			particle::enum_particle_screen_alignment	screen_alignment,
			float4x4 const&							transform,
			float4 const&							instance_color
		);

		virtual void destroy( particle::render_particle_emitter_instance*& instance );
		virtual base_scene_ptr get_scene( particle::world& world );
		virtual ~particle_engine( ) { }

	private:
		particle::world*	m_particle_world;
		scene* const		m_scene;
	};

	explicit scene( scene_configuration const& renderer_configuration );
	virtual ~scene( );

	void set_sky_material( material_effects_instance_ptr const& in_material );
	void add_clouds( cloud_parameters const& parameters );
	void update_clouds( cloud_parameters const& parameters );
	void remove_clouds( );
	void build_lpv_geometry( );
	void on_fs_iterator_probes_ready( char* path, vfs::vfs_locked_iterator const& fs_it );
	void remove_unused_environment_cubemaps( );
	particle::world* particle_world( );

	void add_speedtree_instance( speedtree_instance_ptr instance, float4x4 const& transform, bool populate_forest );
	void remove_speedtree_instance( speedtree_instance_ptr instance, bool populate_forest );
	void set_speedtree_instance_transform( speedtree_instance_ptr instance, float4x4 const& transform, bool populate_forest );
	void populate_speedtree_forest( );

	void add_tracer( tracer_model_instance_ptr const& instance, float4x4 const& initialize_transform );
	void update_tracer( tracer_model_instance_ptr const& instance, float4x4 const& new_transform );
	void remove_tracer( tracer_model_instance_ptr const& instance );

	void process_streaming(
		float4x4 const&	projection_matrix,
		float3			viewer_position,
		u32				screen_size_x,
		u32				screen_size_y
	);

	void add_trample( trample_desc const& desc );

	void on_texture_loaded(
		resources::queries_result&	data,
		res_texture_ptr				texture,
		u32							num_mips,
		float						distance
	);

	void gather_streamable_textures( render_model_instance_impl_ptr model, bool update_only );
	void add_model( render_model_instance_impl_ptr v );
	void modify_model( render_model_instance_impl_ptr v );
	void remove_model( render_model_instance_impl_ptr v );
	void unmove_all_models( );

	void select_models(
		float4x4 const&						mat_vp,
		vector< render_surface_instance* >&	selection,
		float3 const&						view_pos,
		u32									surface_flags,
		bool								moved_only
	);

	void update_models( );
	void add_light( u32 id, light_props* props );
	void update_light( u32 id, light_props* props );
	void remove_light( u32 id );
	void update_environment_probe( u32 id, environment_probe_properties const& properties );
	void remove_environment_probe( u32 id );
	void update_sky_ambient_occlusion( u32 id, sky_ambient_occlusion_properties const& properties );
	void remove_sky_ambient_occlusion( u32 id );
	void update_ambient_volume( u32 id, ambient_volume_properties const& properties );
	void remove_ambient_volume( u32 id );
	void update_lpv_occluder( u32 id, float4x4 const& transform );
	void remove_lpv_occluder( u32 id );
	void add_decal( u32 id, decal_properties const& properties );
	void update_decal( u32 id, decal_properties const& properties );
	void remove_decal( u32 id );
	void add_volume_fog( u32 id, volume_fog_parameters const& in_parameters );
	void update_volume_fog( u32 id, volume_fog_parameters const& in_parameters );
	void remove_volume_fog( u32 id );
	void select_volume_fog_instances( float4x4 const& vp, vector< volume_fog_parameters >& out_instances );
	void set_slomo( float time_multiplier );

	void draw_lines( vectora< vertex_colored > const& vertices, vectora< u16 > const& indices );
	void draw_triangles( vectora< vertex_colored > const& vertices, vectora< u16 > const& indices );

	void flush(
		boost::function< void( bool ) > const&	on_draw_scene,
		bool									all_depth_used,
		bool									all_depth_unused
	);

	void select_model( render_model_instance_impl_ptr const& instance, bool is_selected );
	void set_portal_system( resources::unmanaged_resource_ptr pss );
	void draw_portal_system( system_renderer& r, float3 const& view_pos, float4x4 const& vp );
	void test_action_portal_system( );
	void set_grass( grass_world* w );
	void reset_grass( grass_world* w );
	void dump_scene_statistics( ) const;

private:
	void update_lines( u32 add_count );
	void render_lines( bool covering_effect );
	void update_triangles( u32 add_count );
	void render_triangles( bool covering_effect );

	void select_models_via_sectors(
		float4x4 const&						mat_vp,
		float3 const&						view_point,
		vector< render_surface_instance* >&	selection
	);
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_H_INCLUDED
