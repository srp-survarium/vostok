#ifndef VOSTOK_RENDER_ENGINE_STAGE_VISIBILITY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_VISIBILITY_H_INCLUDED

#include <vostok/math_float4.h>

#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;
struct hw_hiz_occlusion_manager;

class stage_visibility : public stage {
public:
	stage_visibility( renderer* in_renderer, renderer_context* context );
	virtual ~stage_visibility( );

	virtual void execute( ) override;
	virtual void debug_render( ) override;

private:
	void frustum_culling( );
	void occlusion_culling( );
	bool query_data( );

	void get_results_and_prepare_bounds_models( float4*& out_bounds, u32& out_counter );
	void get_results_and_prepare_bounds_lights( float4*& out_bounds, u32& out_counter );
	void get_results_and_prepare_bounds_grass( float4*& out_bounds, u32& out_counter );
	void get_results_and_prepare_bounds_decals( float4*& out_bounds, u32& out_counter );
	void get_results_and_prepare_bounds_env_probes( float4*& out_bounds, u32& out_counter );
	void get_results_and_prepare_bounds_ambient_volumes( float4*& out_bounds, u32& out_counter );
	void get_results_and_prepare_bounds_particles( float4*& out_bounds, u32& out_counter );
	void get_results_and_prepare_bounds_portals( float4*& out_bounds, u32& out_counter );

	bool occluded( u32 const index ) const;
	void gather_statistics( ) const;

private:
	bool						m_data_ready;
	hw_hiz_occlusion_manager*	m_occlusion_manager;
	static s32 const occlusion_buffer_size = 16384;
	float4*						m_static_bounds_array;
	pbyte						m_static_results_array;
	u32							m_current_occlusion_buffer_size;
	u32							m_portals_offset_to_results;
};

STATIC_SIZE_ASSERT( stage_visibility, 0x28 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_VISIBILITY_H_INCLUDED
