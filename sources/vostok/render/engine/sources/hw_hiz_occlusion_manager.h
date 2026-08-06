#ifndef VOSTOK_RENDER_ENGINE_HW_HIZ_OCCLUSION_MANAGER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_HW_HIZ_OCCLUSION_MANAGER_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/fixed_string.h>
#include <vostok/math_float4.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/resource_manager.h>

#include "effect_hiz_occlusion.h"
#include "hw_hiz_point_list.h"
#include "render_target.h"
#include "res_texture.h"
#include "sphere_occluder_geometry.h"

namespace vostok {
namespace render {

class renderer_context;
class shader_constant_host;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

struct hw_hiz_occlusion_manager : public boost::noncopyable {
	hw_hiz_occlusion_manager(
		bool const	use_scene_depth_buffer,
		u32 const	rasterize_width,
		u32 const	rasterize_height
	) :
		m_use_scene_depth_buffer			( use_scene_depth_buffer ),
		m_rasterize_width				( rasterize_width ),
		m_rasterize_height				( rasterize_height ),
		m_num_mips						(
			u32(
				::log( double( math::max( rasterize_width, rasterize_height ) ) ) /
				::log( 2.0 )
			) + 1
		),
		m_source_mip_level_parameter		( 0 ),
		m_draw_color_parameter			( 0 ),
		m_render_target_size_parameter	( 0 ),
		m_rasterize_size_parameter		( 0 ),
		m_prev_texture_size_parameter	( 0 ),
		m_current_num_bounds			( 0 ),
		m_culling_buffer_width			( 0 ),
		m_culling_buffer_height			( 0 )
	{
		for ( u32 mip_index = 0; mip_index < m_num_mips; ++mip_index ) {
			fixed_string<128> name;
			name.assignf(
				"%s_work_%d",
				"$user$hiz_occlusion_depth_mips",
				mip_index
			);
			m_rt_depth_mips_work[mip_index] = resource_manager::ref( ).create_render_target(
				name.c_str( ),
				rasterize_width >> mip_index,
				rasterize_height >> mip_index,
				DXGI_FORMAT_R32_FLOAT,
				enum_rt_usage_render_target,
				res_texture_ptr( ),
				0,
				D3D11_USAGE_DEFAULT,
				1,
				0
			);
			m_t_depth_mips_work[mip_index] =
				m_rt_depth_mips_work[mip_index]->get_texture( );
		}

		m_t_depth_mips = resource_manager::ref( ).create_texture2d(
			"$user$hiz_occlusion_depth_mips",
			rasterize_width,
			rasterize_height,
			0,
			DXGI_FORMAT_R32_FLOAT,
			D3D11_USAGE_DEFAULT,
			m_num_mips,
			1,
			true
		);
		m_t_depth_mips_lockable = resource_manager::ref( ).create_texture2d(
			"$user$hiz_occlusion_depth_mips_l",
			rasterize_width,
			rasterize_height,
			0,
			DXGI_FORMAT_R32_FLOAT,
			D3D11_USAGE_STAGING,
			1,
			1,
			false
		);

		for ( u32 mip_index = 0; mip_index < m_num_mips; ++mip_index ) {
			u32 current_rasterize_height = rasterize_height >> mip_index;
			fixed_string<128> name;
			name.assignf( "%s_%d", "$user$hiz_occlusion_depth_mips", mip_index );
			m_rt_depth_mips[mip_index] = resource_manager::ref( ).create_render_target(
				name.c_str( ),
				rasterize_width >> mip_index,
				current_rasterize_height,
				DXGI_FORMAT_R32_FLOAT,
				enum_rt_usage_render_target,
				m_t_depth_mips,
				0,
				D3D11_USAGE_DEFAULT,
				1,
				mip_index
			);
		}

		m_ds_occlusion_depth = resource_manager::ref( ).create_render_target(
			"$user$hiz_occlusion_depth_mips_d",
			rasterize_width,
			rasterize_height,
			DXGI_FORMAT_R24G8_TYPELESS,
			enum_rt_usage_depth_stencil,
			res_texture_ptr( ),
			0,
			D3D11_USAGE_DEFAULT,
			1,
			0
		);

		effect_manager::ref( ).create_effect<effect_hiz_occlusion>( &m_hiz_occlusion_effect );
		m_source_mip_level_parameter =
			backend::ref( ).register_constant_host( "source_mip_level", rc_int );
		m_draw_color_parameter =
			backend::ref( ).register_constant_host( "draw_color", rc_float );
		m_render_target_size_parameter =
			backend::ref( ).register_constant_host( "render_target_size", rc_float );
		m_rasterize_size_parameter =
			backend::ref( ).register_constant_host( "rasterize_size", rc_float );
		m_prev_texture_size_parameter =
			backend::ref( ).register_constant_host( "prev_texture_size", rc_float );
	}

	~hw_hiz_occlusion_manager( );

	void process_culling(
		renderer_context*	in_context,
		float4 const*		in_bounds,
		u32 const			in_num_bounds_and_results
	);

	bool is_ready_effects( ) const;

	void render_debug(
		renderer_context*	in_context,
		float4 const*		in_bounds,
		pcbyte				in_results,
		u32 const			in_num_bounds_and_results
	);

	bool quary_and_get_results_if_ready( pbyte out_results, u32 const in_num_results );

private:
	void render_occluders( renderer_context* in_context );
	void copy_scene_depth( );
	void downsample_occlusion_buffer( );
	void render_model_bounds(
		renderer_context*	in_context,
		float4 const*		in_bounds,
		u32 const			in_num_bounds
	);
	void check_culling_buffer( u32 const in_num_bounds );

private:
	bool const					m_use_scene_depth_buffer;
	res_effect_ptr				m_hiz_occlusion_effect;
	render_target_ptr			m_rt_depth_mips_work[16];
	res_texture_ptr				m_t_depth_mips_work[16];
	render_target_ptr			m_rt_depth_mips[16];
	render_target_ptr			m_ds_occlusion_depth;
	res_texture_ptr				m_t_depth_mips;
	res_texture_ptr				m_t_depth_mips_copy;
	res_texture_ptr				m_t_depth_mips_lockable;
	u32 const					m_rasterize_width;
	u32 const					m_rasterize_height;
	u32 const					m_num_mips;
	sphere_occluder_geometry	m_sphere_occluder_geometry;
	shader_constant_host*		m_source_mip_level_parameter;
	shader_constant_host*		m_draw_color_parameter;
	shader_constant_host*		m_render_target_size_parameter;
	shader_constant_host*		m_rasterize_size_parameter;
	shader_constant_host*		m_prev_texture_size_parameter;
	u32							m_current_num_bounds;
	u32							m_culling_buffer_width;
	u32							m_culling_buffer_height;
	render_target_ptr			m_rt_culling_result;
	res_texture_ptr				m_t_culling_result;
	res_texture_ptr				m_t_culling_result_lockable;
	hw_hiz_point_list			m_hw_hiz_point_list;
};

STATIC_SIZE_ASSERT( hw_hiz_occlusion_manager, 0x120 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_HW_HIZ_OCCLUSION_MANAGER_H_INCLUDED
