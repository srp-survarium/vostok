////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_TRANSLUCENCY_H_INCLUDED
#define RENDER_STAGE_TRANSLUCENCY_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_translucency : public stage {
public:
	inline			stage_translucency	( renderer* arg_0, renderer_context* arg_1 ) { /* no source */ }

	virtual	void	execute				( ) override;

			bool	is_effects_ready	( ) const;

	virtual			~stage_translucency	( ) { /* no source */ }

	/* 0x0000 */	/* stage */
	/* 0x0010 */	res_effect_ptr				m_translucency_effect;
	/* 0x0014 */	shader_constant_host*		m_shadow[4];
	/* 0x0024 */	shader_constant_host*		m_c_sun_direction;
	/* 0x0028 */	shader_constant_host*		m_c_sun_color;
	/* 0x002c */	shader_constant_host*		m_c_eye_ray_corner;
}; // class stage_translucency

STATIC_SIZE_ASSERT(stage_translucency, 0x30);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_TRANSLUCENCY_H_INCLUDED
