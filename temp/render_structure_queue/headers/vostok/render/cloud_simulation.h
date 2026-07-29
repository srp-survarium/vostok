////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CLOUD_SIMULATION_H_INCLUDED
#define RENDER_CLOUD_SIMULATION_H_INCLUDED

/* INCLUDES */
struct vostok::render::cloud_simulation::voxel;

/* FORWARD REFS */
class vostok::render::cloud_key_parameters;

namespace vostok {
namespace render {

class cloud_simulation : public boost::noncopyable {
public:
												cloud_simulation			( const u32 in_size_x, const u32 in_size_y, const u32 in_size_z );
												~cloud_simulation			( );

			cloud_simulation::voxel*			get_voxels					( ) const;

			void								generate					( cloud_key_parameters const& init_key, float3 const& sun_direction );

			void								copy_from					( cloud_simulation const& other );

private:
			void								set_voxel					(
													cloud_simulation::voxel const&		v,
													const u32							x,
													const u32							y,
													const u32							z
												);
	inline	void								set_voxel					(
													cloud_simulation::voxel*			arg_0,
													cloud_simulation::voxel const&		arg_1,
													const u32							arg_2,
													const u32							arg_3,
													const u32							arg_4
												) { /* no source */ }
			cloud_simulation::voxel const&		get_voxel					( const u32 x, const u32 y, const u32 z ) const;
	inline	cloud_simulation::voxel const&		get_voxel					(
													cloud_simulation::voxel*	arg_0,
													const u32					arg_1,
													const u32					arg_2,
													const u32					arg_3
												) const { /* no source */ }
	inline	cloud_simulation::voxel				get_interp_voxel			( float arg_0, float arg_1, float arg_2 ) const { /* no source */ }
	inline	cloud_simulation::voxel				get_interp_voxel2			( float arg_0, float arg_1, float arg_2 ) const { /* no source */ }

			bool								is_empty					( const u32 x, const u32 y, const u32 z ) const;

			bool								in_grid						( const u32 y, const u32 z, const u32 arg_2 ) const;

			void								smooth_transparency			( );
			void								smooth_voxels_around		( );

			void								fill_default_volume			( );

	inline	float								get_cloud_density			( const u32 arg_0, const u32 arg_1, const u32 arg_2 ) { /* no source */ }
	inline	float								get_cloud_density_near		( const u32 arg_0, const u32 arg_1, const u32 arg_2 ) { /* no source */ }
	inline	float								get_cloud_density_interp	( const float arg_0, const float arg_1, const float arg_2 ) { /* no source */ }

			void								compute_direct_light		( float3 const& sun_direction, cloud_key_parameters const& init_key );
	inline	void								compute_ambient_light		( float3 const& arg_0, cloud_key_parameters const& arg_1 ) { /* no source */ }
			void								compute_indirect_light		( float3 const& sun_direction, cloud_key_parameters const& init_key );
	inline	void								compute_ambient_occlusion	( cloud_key_parameters const& arg_0 ) { /* no source */ }
			void								compute_cloud_density		( );

	/* 0x0000 */	/* boost::noncopyable */
public:
	/* 0x0000 */	float3						cloud_offset;
	/* 0x000c */	float4x4					world_to_cloud;
	/* 0x004c */	float						interp_alpha;
private:
	/* 0x0050 */	cloud_simulation::voxel*	m_voxels;
	/* 0x0054 */	float*						m_densities;
	/* 0x0058 */	const u32					m_clouds_size_x;
	/* 0x005c */	const u32					m_clouds_size_y;
	/* 0x0060 */	const u32					m_clouds_size_z;
}; // class cloud_simulation

STATIC_SIZE_ASSERT(cloud_simulation, 0x64);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CLOUD_SIMULATION_H_INCLUDED
