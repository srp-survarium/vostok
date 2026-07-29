////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_DEBUG_H_INCLUDED
#define RENDER_STAGE_DEBUG_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
struct vostok::render::sphere_geometry;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_debug : public stage {
public:
	inline			stage_debug						( renderer* arg_0, renderer_context* arg_1 ) { /* no source */ }
	virtual			~stage_debug					( );

	virtual	void	execute							( ) override;

			void	render_environment_probe_preview( );

			bool	is_effects_ready				( ) const;

	/* 0x0000 */	/* stage */
	/* 0x0010 */	res_effect_ptr				m_debug_environment_probe_preview_effect;
	/* 0x0014 */	sphere_geometry				m_sphere_geometry;
	/* 0x0028 */	shader_constant_host*		m_preview_mip_index_parameter;
}; // class stage_debug

STATIC_SIZE_ASSERT(stage_debug, 0x2C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_DEBUG_H_INCLUDED
