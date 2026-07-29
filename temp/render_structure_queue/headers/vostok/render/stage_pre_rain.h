////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_PRE_RAIN_H_INCLUDED
#define RENDER_STAGE_PRE_RAIN_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
typedef vostok::intrusive_ptr<vostok::render::render_target,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::render_target_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::render_target;
class vostok::render::res_effect;
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_pre_rain : public stage {
public:
						stage_pre_rain			( renderer* in_renderer, renderer_context* context );
	virtual				~stage_pre_rain			( );

	virtual	void		execute					( ) override;

private:
			float3		compute_aligment		( float3 const& lightXZshift, float4x4 const& light_space_transform, float smap_res );

			bool		is_effects_ready		( ) const;

			float4x4	render_rain_shadow_map	( );

	/* 0x0000 */	/* stage */
	/* 0x0010 */	render_target_ptr			m_rt_rain_shadow_map;
	/* 0x0014 */	res_texture_ptr				m_t_rain_shadow_map;
	/* 0x0018 */	res_effect_ptr				m_wet_surface_effect;
	/* 0x001c */	res_effect_ptr				m_effect_shadow_direct;
	/* 0x0020 */	u32							m_shadow_map_size;
	/* 0x0024 */	shader_constant_host*		m_view_to_shadow_parameter;
	/* 0x0028 */	shader_constant_host*		m_eye_ray_corner_parameter;
	/* 0x002c */	shader_constant_host*		m_rain_offset_parameter;
	/* 0x0030 */	shader_constant_host*		m_rain_density_parameter;
	/* 0x0034 */	float						m_rain_offset;
	/* 0x0038 */	float						m_rain_offset_counter;
}; // class stage_pre_rain

STATIC_SIZE_ASSERT(stage_pre_rain, 0x3C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_PRE_RAIN_H_INCLUDED
