////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_H_INCLUDED
#define RENDER_SCENE_H_INCLUDED

/* INCLUDES */
class associative_vector<unsigned int,vostok::math::float4x4,vostok::render::vector,stlp_std::less<unsigned int> >;
class associative_vector<unsigned int,vostok::render::volume_fog_parameters,vostok::render::vector,stlp_std::less<unsigned int> >;
class vostok::render::culling::portal_sector_system;
class vostok::render::unique_ptr<vostok::render::lights_db>;
class vostok::render::vector<unsigned short>;
class vostok::render::vector<vostok::render::ambient_volume *>;
class vostok::render::vector<vostok::render::environment_probe *>;
class vostok::render::vector<vostok::render::requested_streamable_texture>;
class vostok::render::vector<vostok::render::sky_ambient_occlusion *>;
class vostok::render::vector<vostok::render::streamable_texture_info>;
class vostok::render::vector<vostok::render::streaming_ready_texture>;
class vostok::render::vector<vostok::render::vertex_colored>;
struct vostok::collision::space_partitioning_tree;
struct vostok::render::base_scene;
struct vostok::render::clouds;
struct vostok::render::grass_world;
struct vostok::render::lpv_batched_geometry;
struct vostok::render::scene::particle_engine;
struct vostok::render::shadow_batched_geometry;
struct vostok::render::speedtree_forest;
class vostok::render::res_texture;
typedef vostok::intrusive_list<vostok::render::scene::decal_instance_node,vostok::render::scene::decal_instance_node *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>
	vostok::render::scene::decal_instance_node_list;
typedef vostok::render::vector<vostok::resources::resource_ptr<vostok::render::render_model_instance_impl,vostok::resources::unmanaged_intrusive_base> >
	vostok::render::vector<vostok::render::render_model_instance_impl_ptr >;
typedef vostok::render::vector<vostok::resources::resource_ptr<vostok::render::speedtree_instance,vostok::resources::unmanaged_intrusive_base> >
	vostok::render::vector<vostok::render::speedtree_instance_ptr >;
typedef vostok::render::vector<vostok::resources::resource_ptr<vostok::render::tracer_model_instance,vostok::resources::unmanaged_intrusive_base> >
	vostok::render::vector<vostok::render::tracer_model_instance_ptr >;
typedef vostok::render::vector<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> >
	vostok::render::vector<vostok::resources::unmanaged_resource_ptr >;
typedef vostok::resources::resource_ptr<vostok::particle::world,vostok::resources::unmanaged_intrusive_base>
	vostok::particle::world_ptr;
typedef vostok::resources::resource_ptr<vostok::render::material_effects_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_effects_instance_ptr;
class vostok::particle::world;
class vostok::render::material_effects_instance;
class vostok::render::render_model_instance_impl;
class vostok::render::scene::decal_instance_node;
class vostok::render::speedtree_instance;
class vostok::render::tracer_model_instance;
class vostok::resources::unmanaged_resource;

/* FORWARD REFS */
class vostok::render::ambient_volume_properties;
class vostok::render::cloud_parameters;
class vostok::render::decal_properties;
class vostok::render::environment_probe_properties;
class vostok::render::light_props;
class vostok::render::lights_db;
class vostok::render::scene_configuration;
class vostok::render::sky_ambient_occlusion_properties;
class vostok::render::system_renderer;
class vostok::render::trample_desc;
class vostok::render::vector<vostok::render::render_surface_instance *>;
class vostok::render::vector<vostok::render::volume_fog_parameters>;
class vostok::render::volume_fog_parameters;
class vostok::resources::queries_result;
class vostok::vectora<unsigned short>;
class vostok::vectora<vostok::render::vertex_colored>;
class vostok::vfs::vfs_locked_iterator;

namespace vostok {
namespace render {

class scene : public base_scene {
public:
			explicit								scene								( scene_configuration const& renderer_configuration );
	virtual											~scene								( );

			void									add_speedtree_instance				( speedtree_instance_ptr instance, float4x4 const& transform, bool populate_forest );
			void									remove_speedtree_instance			( speedtree_instance_ptr instance, bool populate_forest );

			void									set_speedtree_instance_transform	( speedtree_instance_ptr instance, float4x4 const& transform, bool populate_forest );

			void									populate_speedtree_forest			( );

			void									add_model							( render_model_instance_impl_ptr v );
			void									modify_model						( render_model_instance_impl_ptr v );
			void									remove_model						( render_model_instance_impl_ptr v );

			void									add_light							( u32 id, light_props* props );
			void									update_light						( u32 id, light_props* props );
			void									remove_light						( u32 id );

			void									add_decal							( u32 id, decal_properties const& properties );
			void									update_decal						( u32 id, decal_properties const& properties );
			void									remove_decal						( u32 id );

			void									update_environment_probe			( u32 id, environment_probe_properties const& properties );
			void									remove_environment_probe			( u32 id );

			void									update_sky_ambient_occlusion		( u32 id, sky_ambient_occlusion_properties const& properties );
			void									remove_sky_ambient_occlusion		( u32 id );

			void									update_ambient_volume				( u32 id, ambient_volume_properties const& properties );
			void									remove_ambient_volume				( u32 id );

			void									update_lpv_occluder					( u32 id, float4x4 const& transform );
			void									remove_lpv_occluder					( u32 id );

			void									add_volume_fog						( u32 id, volume_fog_parameters const& in_parameters );
			void									update_volume_fog					( u32 id, volume_fog_parameters const& in_parameters );
			void									remove_volume_fog					( u32 id );

			void									select_models						(
														float4x4 const&							mat_vp,
														vector< render_surface_instance* >&		selection,
														float3 const&							view_pos,
														u32										surface_flags,
														bool									moved_only
													);
			void									update_models						( );

			void									add_tracer							( tracer_model_instance_ptr const& instance, float4x4 const& initialize_transform );
			void									update_tracer						( tracer_model_instance_ptr const& instance, float4x4 const& new_transform );
			void									remove_tracer						( tracer_model_instance_ptr const& instance );

	inline	void									select_particle_system_instance		( resources::unmanaged_resource_ptr const& arg_0, bool arg_1 ) { /* no source */ }
	inline	void									select_speedtree_instance			( speedtree_instance_ptr const& arg_0, bool arg_1 ) { /* no source */ }
			void									select_model						( render_model_instance_impl_ptr const& instance, bool is_selected );

			void									add_clouds							( cloud_parameters const& parameters );
			void									update_clouds						( cloud_parameters const& parameters );
			void									remove_clouds						( );

			void									build_lpv_geometry					( );
	inline	void									build_lpv_occluder_geometry			( ) { /* no source */ }

			void									remove_unused_environment_cubemaps	( );

			void									on_fs_iterator_probes_ready			( char* path, vfs::vfs_locked_iterator const& fs_it );

			void									set_portal_system					( resources::unmanaged_resource_ptr pss );
	inline	void									remove_portal_system				( ) { /* no source */ }
			void									draw_portal_system					( system_renderer& r, float3 const& view_pos, float4x4 const& vp );
			void									test_action_portal_system			( );

	inline	u32										get_portals_count					( ) const { /* no source */ }
	inline	void									get_portals_occlusion_bounds		( float4* arg_0 ) const { /* no source */ }

	inline	void									update_portals_occlusion_culling	( pcbyte arg_0 ) { /* no source */ }

			void									unmove_all_models					( );

			void									draw_lines							( vectora< vertex_colored > const& vertices, vectora< u16 > const& indices );
			void									draw_triangles						( vectora< vertex_colored > const& vertices, vectora< u16 > const& indices );

			void									flush								(
														boost::function< void( bool ) > const&	on_draw_scene,
														bool	all_depth_used,
														bool	all_depth_unused
													);

			void									process_streaming					(
														float4x4 const&		projection_matrix,
														float3				viewer_position,
														const u32			screen_size_x,
														const u32			screen_size_y
													);

			void									gather_streamable_textures			( render_model_instance_impl_ptr model, bool update_only );

			void									on_texture_loaded					(
														resources::queries_result&		data,
														res_texture_ptr					texture,
														u32								num_mips,
														float							distance
													);

	inline	lights_db&								lights								( ) { /* no source */ }

			particle::world*						particle_world						( );

	inline	speedtree_forest*						get_speedtree_forest				( ) { /* no source */ }

			void									set_slomo							( float time_multiplier );
	inline	float									get_slomo							( ) const { /* no source */ }

			void									add_trample							( trample_desc const& desc );

	inline	grass_world*							get_grass							( ) const { /* no source */ }
			void									set_grass							( grass_world* w );
			void									reset_grass							( grass_world* w );

			void									set_sky_material					( material_effects_instance_ptr const& in_material );
	inline	material_effects_instance_ptr const&	get_sky_material					( ) const { /* no source */ }

	inline	bool									has_clouds							( ) const { /* no source */ }
	inline	clouds*									get_clouds							( ) const { /* no source */ }

	inline	bool									sky_enabled							( ) const { /* no source */ }

	inline	bool									use_occlusion_culling				( ) const { /* no source */ }

			void									dump_scene_statistics				( ) const;

private:
	inline	scene*									return_this							( ) { /* no source */ }

			void									update_lines						( u32 add_count );
			void									render_lines						( bool covering_effect );

			void									update_triangles					( u32 add_count );
			void									render_triangles					( bool covering_effect );

			void									select_models_via_sectors			(
														float4x4 const&							mat_vp,
														float3 const&							view_point,
														vector< render_surface_instance* >&		selection
													);
public:
			void									select_volume_fog_instances			( float4x4 const& vp, vector< volume_fog_parameters >& out_instances );

	inline	scene::decal_instance_node_list&		decals								( ) { /* no source */ }

	inline	collision::space_partitioning_tree&		decals_tree							( ) { /* no source */ }

	inline	vector< environment_probe* >&			environment_probes					( ) { /* no source */ }
	inline	collision::space_partitioning_tree&		environment_probes_tree				( ) { /* no source */ }

	inline	vector< sky_ambient_occlusion* >&		sky_ao_volumes						( ) { /* no source */ }

	inline	associative_vector< u32, float4x4, vector, std::less< u32 > >&	lpv_occluders						( ) { /* no source */ }

	inline	vector< tracer_model_instance_ptr >&	tracers								( ) { /* no source */ }

	inline	vector< ambient_volume* >&				ambient_volumes						( ) { /* no source */ }

	inline	lpv_batched_geometry&					get_lpv_geometry					( ) { /* no source */ }
	inline	shadow_batched_geometry&				get_shadow_geometry					( ) { /* no source */ }

	/* 0x0000 */	/* base_scene */
	/* 0x0118 */	vector< streamable_texture_info >		streaming_textures;
	/* 0x0124 */	vector< requested_streamable_texture >	requested_streamable_textures;
	/* 0x0130 */	vector< streaming_ready_texture >		ready_streaming_textures;
	/* 0x013c */	associative_vector< u32, volume_fog_parameters, vector, std::less< u32 > >	m_volume_fogs;
private:
	/* 0x014c */	material_effects_instance_ptr			m_sky_material;
	/* 0x0150 */	lpv_batched_geometry					m_lpv_geometry;
	/* 0x021c */	shadow_batched_geometry					m_shadow_geometry;
	/* 0x02e8 */	scene::particle_engine					m_particle_engine;
	/* 0x02f4 */	vector< vertex_colored >				m_line_vertices;
	/* 0x0300 */	vector< u16 >							m_line_indices;
	/* 0x030c */	vector< vertex_colored >				m_triangle_vertices;
	/* 0x0318 */	vector< u16 >							m_triangle_indices;
	/* 0x0324 */	vector< render_model_instance_impl_ptr >	m_selected_models;
	/* 0x0330 */	vector< resources::unmanaged_resource_ptr >	m_particle_system_instances;
	/* 0x033c */	vector< speedtree_instance_ptr >		m_speedtree_instances;
	/* 0x0348 */	vector< tracer_model_instance_ptr >		m_tracers;
	/* 0x0354 */	vector< sky_ambient_occlusion* >		m_sky_ao_volumes;
	/* 0x0360 */	vector< ambient_volume* >				m_ambient_volumes;
	/* 0x036c */	scene::decal_instance_node_list			m_decals;
	/* 0x037c */	vector< environment_probe* >			m_environment_probes;
	/* 0x0388 */	collision::space_partitioning_tree*		m_decals_tree;
	/* 0x038c */	collision::space_partitioning_tree*		m_environment_probes_tree;
	/* 0x0390 */	collision::space_partitioning_tree*		m_models_tree;
	/* 0x0394 */	associative_vector< u32, float4x4, vector, std::less< u32 > >	m_lpv_occluders;
	/* 0x03a4 */	vector< render_model_instance_impl_ptr >	m_render_model_instances;
	/* 0x03b0 */	unique_ptr< lights_db >					m_lights;
	/* 0x03b4 */	particle::world_ptr						m_particle_world;
	/* 0x03b8 */	speedtree_forest*						m_speedtree_forest;
	/* 0x03bc */	grass_world*							m_grass;
	/* 0x03c0 */	clouds*									m_clouds;
	/* 0x03c4 */	float									m_scene_slomo;
	/* 0x03c8 */	bool									m_use_clouds;
	/* 0x03c9 */	bool									m_sky_enabled;
	/* 0x03ca */	bool									m_use_occlusion_culling;
	/* 0x03cc */	culling::portal_sector_system*			m_portal_system;
}; // class scene

STATIC_SIZE_ASSERT(scene, 0x3D0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_H_INCLUDED
