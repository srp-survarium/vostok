////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_DECALS_ACCUMULATE_H_INCLUDED
#define RENDER_STAGE_DECALS_ACCUMULATE_H_INCLUDED

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

class stage_decals_accumulate : public stage {
public:
	inline			stage_decals_accumulate	( renderer* arg_0, renderer_context* arg_1 ) { /* no source */ }
	virtual			~stage_decals_accumulate( );

	virtual	void	execute					( ) override;

	virtual	void	execute_disabled		( ) override;

	virtual	void	debug_render			( ) override;

private:
			bool	is_effects_ready		( ) const;

	/* 0x0000 */	/* stage */
	/* 0x0010 */	res_effect_ptr		m_opaque_geometry_mask_effect;
	/* 0x0014 */	res_effect_ptr		m_apply_decal_effect;
}; // class stage_decals_accumulate

STATIC_SIZE_ASSERT(stage_decals_accumulate, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_DECALS_ACCUMULATE_H_INCLUDED
