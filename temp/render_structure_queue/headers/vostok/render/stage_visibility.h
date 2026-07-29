////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_VISIBILITY_H_INCLUDED
#define RENDER_STAGE_VISIBILITY_H_INCLUDED

/* INCLUDES */
class vostok::render::stage;
struct vostok::render::hw_hiz_occlusion_manager;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_visibility : public stage {
public:
					stage_visibility	( renderer* in_renderer, renderer_context* context );
	virtual			~stage_visibility	( );

	virtual	void	execute				( ) override;

	virtual	void	debug_render		( ) override;

private:
			void	frustum_culling		( );
			void	occlusion_culling	( );

			bool	query_data			( );

			void	get_results_and_prepare_bounds_models( float4*& out_bounds, u32& out_counter );
			void	get_results_and_prepare_bounds_lights( float4*& out_bounds, u32& out_counter );
			void	get_results_and_prepare_bounds_grass( float4*& out_bounds, u32& out_counter );
			void	get_results_and_prepare_bounds_decals( float4*& out_bounds, u32& out_counter );
			void	get_results_and_prepare_bounds_env_probes( float4*& out_bounds, u32& out_counter );
			void	get_results_and_prepare_bounds_ambient_volumes( float4*& out_bounds, u32& out_counter );
			void	get_results_and_prepare_bounds_particles( float4*& out_bounds, u32& out_counter );
			void	get_results_and_prepare_bounds_portals( float4*& out_bounds, u32& out_counter );

			bool	occluded			( const u32 index ) const;

			void	gather_statistics	( ) const;

	/* 0x0000 */	/* stage */
	/* 0x0010 */	bool							m_data_ready;
	/* 0x0014 */	hw_hiz_occlusion_manager*		m_occlusion_manager;
	/* 0x0018 */	float4*							m_static_bounds_array;
	/* 0x001c */	pbyte							m_static_results_array;
	/* 0x0020 */	u32								m_current_occlusion_buffer_size;
	/* 0x0024 */	u32								m_portals_offset_to_results;
}; // class stage_visibility

STATIC_SIZE_ASSERT(stage_visibility, 0x28);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_VISIBILITY_H_INCLUDED
