#ifndef VOSTOK_RENDER_ENGINE_SCENE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_H_INCLUDED

#include <vostok/associative_vector.h>
#include <vostok/intrusive_list.h>
#include <vostok/particle/engine.h>
#include <vostok/particle/world.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/engine/vertex_colored.h>
#include <vostok/render/facade/volume_fog_parameters.h>
#include <vostok/vectora.h>

#include "decal_instance.h"
#include "lights_db.h"
#include "lpv_batched_geometry.h"
#include "material_effects_instance.h"
#include "render_model_instance_impl.h"
#include "shadow_batched_geometry.h"
#include "sky_ambient_occlusion.h"
#include "speedtree_instance.h"
#include "streaming.h"
#include "tracer_model_instance.h"

namespace vostok {

namespace collision {
struct space_partitioning_tree;
}

namespace particle {

struct world;

typedef particle_list_type base_particle_list;
typedef resources::resource_ptr<
	world,
	resources::unmanaged_intrusive_base
> world_ptr;

} // namespace particle

namespace vfs {
class vfs_locked_iterator;
}

namespace render {

namespace culling {
class portal_sector_system;
}

class ambient_volume;
class clouds;
class environment_probe;
class render_surface_instance;
class system_renderer;

struct ambient_volume_properties;
struct cloud_parameters;
struct decal_properties;
struct environment_probe_properties;
struct grass_patch;
struct grass_world;
struct scene_configuration;
struct speedtree_forest;
struct trample_desc;

typedef intrusive_ptr<
	decal_instance,
	resource_intrusive_base,
	threading::single_threading_policy
> decal_instance_ptr;

class scene : public base_scene {
public:
	struct particle_engine :
		public particle::engine,
		public boost::noncopyable
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

		virtual particle::render_particle_emitter_instance*
		create_render_emitter_instance(
			particle::particle_emitter_instance& particle_emitter_instance,
			particle::base_particle_list const& particle_list,
			particle::billboard_parameters* billboard_parameters,
			particle::beamtrail_parameters* beamtrail_parameters,
			particle::enum_particle_locked_axis locked_axis,
			particle::enum_particle_screen_alignment screen_alignment,
			float4x4 const& transform,
			float4 const& instance_color
		);

		virtual void destroy(
			particle::render_particle_emitter_instance*& instance
		);
		virtual base_scene_ptr get_scene( particle::world& world );
		virtual ~particle_engine( ) { }

		particle::world* m_particle_world;

	private:
		scene* const m_scene;
	};

	struct decal_instance_node {
		explicit decal_instance_node( decal_instance* instance ) :
			decal( instance ),
			next( 0 )
		{
		}

		~decal_instance_node( ) { }

		decal_instance_ptr decal;
		decal_instance_node* next;
	};

	typedef intrusive_list<
		decal_instance_node,
		decal_instance_node*,
		&decal_instance_node::next,
		threading::single_threading_policy,
		size_policy,
		no_debug_policy
	> decal_instance_node_list;

public:
	explicit scene( scene_configuration const& renderer_configuration );
	virtual ~scene( );

	void add_speedtree_instance(
		speedtree_instance_ptr instance,
		float4x4 const& transform,
		bool populate_forest
	);
	void remove_speedtree_instance(
		speedtree_instance_ptr instance,
		bool populate_forest
	);
	void set_speedtree_instance_transform(
		speedtree_instance_ptr instance,
		float4x4 const& transform,
		bool populate_forest
	);
	void populate_speedtree_forest( );

	void add_model( render_model_instance_impl_ptr model );
	void modify_model( render_model_instance_impl_ptr model );
	void remove_model( render_model_instance_impl_ptr model );

	void add_light( u32 id, light_props* properties );
	void update_light( u32 id, light_props* properties );
	void remove_light( u32 id );

	void add_decal( u32 id, decal_properties const& properties );
	void update_decal( u32 id, decal_properties const& properties );
	void remove_decal( u32 id );

	void update_environment_probe(
		u32 id,
		environment_probe_properties const& properties
	);
	void remove_environment_probe( u32 id );

	void update_sky_ambient_occlusion(
		u32 id,
		sky_ambient_occlusion_properties const& properties
	);
	void remove_sky_ambient_occlusion( u32 id );

	void update_ambient_volume(
		u32 id,
		ambient_volume_properties const& properties
	);
	void remove_ambient_volume( u32 id );

	void update_lpv_occluder( u32 id, float4x4 const& transform );
	void remove_lpv_occluder( u32 id );

	void add_volume_fog( u32 id, volume_fog_parameters const& parameters );
	void update_volume_fog( u32 id, volume_fog_parameters const& parameters );
	void remove_volume_fog( u32 id );

	void select_models(
		float4x4 const& matrix,
		vector< render_surface_instance* >& selection,
		float3 const& view_position,
		u32 surface_flags,
		bool moved_only
	);
	void update_models( );

	void add_tracer(
		tracer_model_instance_ptr const& instance,
		float4x4 const& initial_transform
	);
	void update_tracer(
		tracer_model_instance_ptr const& instance,
		float4x4 const& transform
	);
	void remove_tracer( tracer_model_instance_ptr const& instance );

	void select_particle_system_instance(
		resources::unmanaged_resource_ptr const& instance,
		bool selected
	);
	void select_speedtree_instance(
		speedtree_instance_ptr const& instance,
		bool selected
	);
	void select_model(
		render_model_instance_impl_ptr const& instance,
		bool selected
	);

	void add_clouds( cloud_parameters const& parameters );
	void update_clouds( cloud_parameters const& parameters );
	void remove_clouds( );

	void build_lpv_geometry( );
	void build_lpv_occluder_geometry( );
	void remove_unused_environment_cubemaps( );
	void on_fs_iterator_probes_ready(
		char* path,
		vfs::vfs_locked_iterator const& iterator
	);

	void set_portal_system( resources::unmanaged_resource_ptr system );
	void remove_portal_system( ) { m_portal_system = 0; }
	void draw_portal_system(
		system_renderer& renderer,
		float3 const& view_position,
		float4x4 const& view_projection
	);
	void test_action_portal_system( );

	u32 get_portals_count( ) const;
	void get_portals_occlusion_bounds( float4* bounds ) const;
	void update_portals_occlusion_culling( pcbyte results );

	void unmove_all_models( );
	void draw_lines(
		vectora< vertex_colored > const& vertices,
		vectora< u16 > const& indices
	);
	void draw_triangles(
		vectora< vertex_colored > const& vertices,
		vectora< u16 > const& indices
	);

	void flush(
		boost::function< void( bool ) > const& on_draw_scene,
		bool all_depth_used,
		bool all_depth_unused
	);

	void process_streaming(
		float4x4 const& projection_matrix,
		float3 viewer_position,
		u32 screen_size_x,
		u32 screen_size_y
	);
	void gather_streamable_textures(
		render_model_instance_impl_ptr model,
		bool update_only
	);
	void on_texture_loaded(
		resources::queries_result& data,
		res_texture_ptr texture,
		u32 num_mips,
		float distance
	);

	lights_db& lights( ) { return *m_lights; }
	particle::world* particle_world( );
	speedtree_forest* get_speedtree_forest( ) { return m_speedtree_forest; }

	void set_slomo( float time_multiplier );
	float get_slomo( ) const { return m_scene_slomo; }
	void add_trample( trample_desc const& description );

	grass_world* get_grass( ) const { return m_grass; }
	void set_grass( grass_world* world );
	void reset_grass( grass_world* world );

	void set_sky_material( material_effects_instance_ptr const& material );
	material_effects_instance_ptr const& get_sky_material( ) const
	{
		return m_sky_material;
	}

	bool has_clouds( ) const { return m_clouds != 0; }
	clouds* get_clouds( ) const { return m_clouds; }
	bool sky_enabled( ) const { return m_sky_enabled; }
	bool use_occlusion_culling( ) const { return m_use_occlusion_culling; }
	void dump_scene_statistics( ) const;

	void select_volume_fog_instances(
		float4x4 const& view_projection,
		vector< volume_fog_parameters >& instances
	);

	decal_instance_node_list& decals( ) { return m_decals; }
	collision::space_partitioning_tree& decals_tree( )
	{
		return *m_decals_tree;
	}

	vector< environment_probe* >& environment_probes( )
	{
		return m_environment_probes;
	}
	collision::space_partitioning_tree& environment_probes_tree( )
	{
		return *m_environment_probes_tree;
	}

	vector< sky_ambient_occlusion* >& sky_ao_volumes( )
	{
		return m_sky_ao_volumes;
	}

	associative_vector< u32, float4x4, vector, std::less< u32 > >&
	lpv_occluders( )
	{
		return m_lpv_occluders;
	}

	vector< tracer_model_instance_ptr >& tracers( ) { return m_tracers; }
	vector< ambient_volume* >& ambient_volumes( ) { return m_ambient_volumes; }
	lpv_batched_geometry& get_lpv_geometry( ) { return m_lpv_geometry; }
	shadow_batched_geometry& get_shadow_geometry( ) { return m_shadow_geometry; }

public:
	vector< streamable_texture_info > streaming_textures;
	vector< requested_streamable_texture > requested_streamable_textures;
	vector< streaming_ready_texture > ready_streaming_textures;
	associative_vector<
		u32,
		volume_fog_parameters,
		vector,
		std::less< u32 >
	> m_volume_fogs;

private:
	friend	class				scene_cook;

	scene* return_this( ) { return this; }
	void update_lines( u32 add_count );
	void render_lines( bool covering_effect );
	void update_triangles( u32 add_count );
	void render_triangles( bool covering_effect );
	void select_models_via_sectors(
		float4x4 const& matrix,
		float3 const& view_point,
		vector< render_surface_instance* >& selection
	);

private:
	material_effects_instance_ptr m_sky_material;
	lpv_batched_geometry m_lpv_geometry;
	shadow_batched_geometry m_shadow_geometry;
	particle_engine m_particle_engine;
	vector< vertex_colored > m_line_vertices;
	vector< u16 > m_line_indices;
	vector< vertex_colored > m_triangle_vertices;
	vector< u16 > m_triangle_indices;
	vector< render_model_instance_impl_ptr > m_selected_models;
	vector< resources::unmanaged_resource_ptr > m_particle_system_instances;
	vector< speedtree_instance_ptr > m_speedtree_instances;
	vector< tracer_model_instance_ptr > m_tracers;
	vector< sky_ambient_occlusion* > m_sky_ao_volumes;
	vector< ambient_volume* > m_ambient_volumes;
	decal_instance_node_list m_decals;
	vector< environment_probe* > m_environment_probes;
	collision::space_partitioning_tree* m_decals_tree;
	collision::space_partitioning_tree* m_environment_probes_tree;
	collision::space_partitioning_tree* m_models_tree;
	associative_vector<
		u32,
		float4x4,
		vector,
		std::less< u32 >
	> m_lpv_occluders;
	vector< render_model_instance_impl_ptr > m_render_model_instances;
	unique_ptr< lights_db > m_lights;
	particle::world_ptr m_particle_world;
	speedtree_forest* m_speedtree_forest;
	grass_world* m_grass;
	clouds* m_clouds;
	float m_scene_slomo;
	bool m_use_clouds;
	bool m_sky_enabled;
	bool m_use_occlusion_culling;
	culling::portal_sector_system* m_portal_system;
};

STATIC_SIZE_ASSERT( scene::particle_engine, 0xC );
STATIC_SIZE_ASSERT( scene::decal_instance_node, 0x8 );
STATIC_SIZE_ASSERT( scene, 0x3D0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_H_INCLUDED
