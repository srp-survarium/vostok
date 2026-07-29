////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_APPLY_DISTORTION_H_INCLUDED
#define RENDER_STAGE_APPLY_DISTORTION_H_INCLUDED

/* INCLUDES */
class vostok::render::stage;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_apply_distortion : public stage {
public:
	inline			stage_apply_distortion	( renderer* arg_0, renderer_context* arg_1 ) { /* no source */ }
	virtual			~stage_apply_distortion	( );

	virtual	void	execute					( ) override;

			bool	is_effects_ready		( ) const;

	/* 0x0000 */	/* stage */
private:
	/* 0x0010 */	res_effect_ptr		m_sh_apply_distortion;
}; // class stage_apply_distortion

STATIC_SIZE_ASSERT(stage_apply_distortion, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_APPLY_DISTORTION_H_INCLUDED
