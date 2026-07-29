////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_RESOLVE_LIGHTING_H_INCLUDED
#define RENDER_STAGE_RESOLVE_LIGHTING_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;
class vostok::render::vector<vostok::render::render_surface_instance *>;

namespace vostok {
namespace render {

class stage_resolve_lighting : public stage {
public:
	inline			stage_resolve_lighting	( renderer* arg_0, renderer_context* arg_1 ) { /* no source */ }

	virtual	void	execute					( ) override;

			bool	is_effects_ready		( ) const;

			void	render_models			( vector< render_surface_instance* >& models, u32& out_num_rendered );

	virtual			~stage_resolve_lighting	( ) { /* no source */ }

	/* 0x0000 */	/* stage */
	/* 0x0010 */	res_effect_ptr				m_resolve_lighting_effect;
	/* 0x0014 */	shader_constant_host*		m_wind_info_parameters;
	/* 0x0018 */	shader_constant_host*		m_sun_light_parameters;
	/* 0x001c */	shader_constant_host*		m_eye_ray_corner_parameter;
}; // class stage_resolve_lighting

STATIC_SIZE_ASSERT(stage_resolve_lighting, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_RESOLVE_LIGHTING_H_INCLUDED
