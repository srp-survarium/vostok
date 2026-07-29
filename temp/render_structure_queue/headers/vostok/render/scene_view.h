////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_VIEW_H_INCLUDED
#define RENDER_SCENE_VIEW_H_INCLUDED

/* INCLUDES */
class vostok::render::camera;
class vostok::render::vector<vostok::render::ambient_volume *>;
class vostok::render::vector<vostok::render::decal_instance *>;
class vostok::render::vector<vostok::render::environment_probe *>;
class vostok::render::vector<vostok::render::grass_patch *>;
class vostok::render::vector<vostok::render::render_surface_instance *>;
class vostok::vectora<vostok::particle::render_particle_emitter_instance *>;
struct vostok::render::base_scene_view;
struct vostok::render::culling_result;
struct vostok::render::post_process_parameters;
struct survarium::flash_text_manager;
class vostok::particle::enum_particle_render_mode;
class vostok::render::scene_view_mode;
typedef vostok::render::vector<vostok::intrusive_ptr<vostok::render::light,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >
	vostok::render::vector<vostok::render::light_ptr >;
typedef vostok::render::vector<vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base> >
	vostok::render::vector<survarium::flash_movie_resource_ptr >;
class vostok::render::light;
class survarium::flash_movie_resource;

namespace vostok {
namespace render {

enum vostok::render::scene_view_mode
{
	wireframe_view_mode					= 0x00,
	wireframe_two_sided_view_mode		= 0x01,
	unlit_view_mode						= 0x02,
	lit_view_mode						= 0x03,
	normals_view_mode					= 0x04,
	specular_color_view_mode			= 0x05,
	translucency_view_mode				= 0x06,
	fresnel_view_mode					= 0x07,
	roughness_view_mode					= 0x08,
	miplevel_view_mode					= 0x09,
	lighting_view_mode					= 0x0a,
	lighting_diffuse_view_mode			= 0x0b,
	lighting_specular_view_mode			= 0x0c,
	lighting_specular_mul_intensity_view_mode	= 0x0d,
	shader_complexity_view_mode			= 0x0e,
	texture_density_view_mode			= 0x0f,
	geometry_complexity_view_mode		= 0x10,
	emissive_only_view_mode				= 0x11,
	distortion_only_view_mode			= 0x12,
	ambient_occlusion_only_view_mode	= 0x13,
	unlit_with_ao_view_mode				= 0x14,
	overdraw_view_mode					= 0x15,
	indirect_lighting_view_mode			= 0x16,
	vertex_alpha_view_mode				= 0x17,
	lpv_geometry_view_mode				= 0x18,
	lighting_luminance_view_mode		= 0x19,
	num_view_modes						= 0x1a,
};
enum vostok::particle::enum_particle_render_mode
{
	normal_particle_render_mode		= 0x0,
	dots_particle_render_mode		= 0x1,
	size_particle_render_mode		= 0x2,
};

class scene_view : public base_scene_view {
public:
													scene_view						( );
	virtual											~scene_view						( );
	inline	void									camera_set_view					( float4x4 const& arg_0 ) { /* no source */ }
	inline	void									camera_set_projection			( float4x4 const& arg_0 ) { /* no source */ }

	inline	camera const&							camera							( ) const { /* no source */ }

	inline	post_process_parameters&				post_process_parameters			( ) { /* no source */ }
	inline	post_process_parameters const&			post_process_parameters			( ) const { /* no source */ }

			void									add_movie						( survarium::flash_movie_resource_ptr& movie );
			void									remove_movie					( survarium::flash_movie_resource_ptr& movie );

			void									add_text_manager				( survarium::flash_text_manager* tm );
			void									remove_text_manager				( survarium::flash_text_manager* tm );

	inline	vector< survarium::flash_movie_resource_ptr > const&	flash_movies					( ) const { /* no source */ }
	inline	survarium::flash_text_manager*			flash_text_manager				( ) const { /* no source */ }

	inline	void									set_view_mode					( scene_view_mode arg_0 ) { /* no source */ }
	inline	scene_view_mode							get_view_mode					( ) const { /* no source */ }
	inline	float4 const&							get_luminance_parameters		( ) const { /* no source */ }
	inline	float4 const&							get_prev_luminance_parameters	( ) const { /* no source */ }
	inline	void									set_luminance_parameters		( float4 const& arg_0 ) { /* no source */ }
	inline	void									set_prev_luminance_parameters	( float4 const& arg_0 ) { /* no source */ }

	inline	u32										get_render_frame_index			( ) const { /* no source */ }

	inline	void									set_use_post_process			( bool arg_0 ) { /* no source */ }
	inline	bool									is_use_post_process				( ) const { /* no source */ }

	inline	void									set_particles_render_mode		( particle::enum_particle_render_mode arg_0 ) { /* no source */ }
	inline	particle::enum_particle_render_mode		get_particles_render_mode		( ) const { /* no source */ }
	inline	vector< render_surface_instance* >&		get_visible_models				( ) { /* no source */ }
	inline	vector< render_surface_instance* >&		get_visible_opaque_models		( ) { /* no source */ }
	inline	vector< render_surface_instance* >&		get_visible_moved_opaque_models	( ) { /* no source */ }
	inline	vector< light_ptr >&					get_visible_lights				( ) { /* no source */ }
	inline	vectora< particle::render_particle_emitter_instance* >&	get_visible_particle_instances	( ) { /* no source */ }
	inline	vector< decal_instance* >&				get_visible_decals				( ) { /* no source */ }
	inline	vector< environment_probe* >&			get_visible_environment_probes	( ) { /* no source */ }
	inline	vector< grass_patch* >&					get_visible_grass_patches		( ) { /* no source */ }
	inline	vector< ambient_volume* >&				get_visible_ambient_volumes		( ) { /* no source */ }
	inline	void									get_histogram_values			( float* arg_0 ) { /* no source */ }
	inline	void									set_histogram_values			( float* arg_0 ) { /* no source */ }

private:
	inline	void									inc_render_frame_index			( ) { /* no source */ }

	/* 0x0000 */	/* base_scene_view */
public:
	/* 0x0118 */	post_process_parameters					m_post_process_parameters;
	/* 0x03ec */	u32										luminance_pass_index;
	/* 0x03f0 */	bool									need_recalc_atmosphere;
private:
	/* 0x03f4 */	camera									m_camera;
	/* 0x04b4 */	scene_view_mode							m_view_mode;
	/* 0x04b8 */	particle::enum_particle_render_mode		m_particles_render_mode;
	/* 0x04bc */	bool									m_use_post_process;
	/* 0x04c0 */	u32										m_render_frame_index;
	/* 0x04c4 */	float4									m_frame_luminance_parameters;
	/* 0x04d4 */	float4									m_prev_frame_luminance_parameters;
	/* 0x04e4 */	float									m_histogram_values[16];
	/* 0x0524 */	vector< render_surface_instance* >		m_visible_moved_models;
	/* 0x0530 */	vector< render_surface_instance* >		m_visible_models;
	/* 0x053c */	vector< render_surface_instance* >		m_visible_opaque_models;
	/* 0x0548 */	vector< light_ptr >						m_visible_lights;
	/* 0x0554 */	vector< decal_instance* >				m_visible_decals;
	/* 0x0560 */	vector< environment_probe* >			m_visible_environment_probes;
	/* 0x056c */	vectora< particle::render_particle_emitter_instance* >	m_visible_particle_instances;
	/* 0x057c */	vector< grass_patch* >					m_visible_grass_patches;
	/* 0x0588 */	vector< ambient_volume* >				m_visible_ambient_volumes;
	/* 0x0594 */	vector< survarium::flash_movie_resource_ptr >	m_flash_movies;
	/* 0x05a0 */	survarium::flash_text_manager*			m_flash_text_manager;
	/* 0x05a4 */	u32										m_num_frustum_culled_surfaces;
	/* 0x05a8 */	u32										m_num_frustum_culled_lights;
	/* 0x05ac */	u32										m_num_frustum_culled_grass_patches;
	/* 0x05b0 */	u32										m_num_frustum_culled_particle_instances;
	/* 0x05b4 */	u32										m_num_frustum_culled_decals;
	/* 0x05b8 */	u32										m_num_occlusion_culled_surfaces;
	/* 0x05bc */	u32										m_num_occlusion_culled_lights;
	/* 0x05c0 */	u32										m_num_occlusion_culled_grass_patches;
	/* 0x05c4 */	u32										m_num_occlusion_culled_particle_instances;
	/* 0x05c8 */	u32										m_num_occlusion_culled_decals;
	/* 0x05cc */	pbyte									m_occlusion_culling_results;
	/* 0x05d0 */	u32										m_num_occlusion_culling_results;
	/* 0x05d4 */	culling_result*							m_culling_results;
	/* 0x05d8 */	float4*									m_culling_bounds;
	/* 0x05dc */	u32										m_atmosphere_prev_targets_size;
}; // class scene_view

STATIC_SIZE_ASSERT(scene_view, 0x5E0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_VIEW_H_INCLUDED
