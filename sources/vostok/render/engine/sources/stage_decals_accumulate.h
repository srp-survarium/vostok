#ifndef VOSTOK_RENDER_ENGINE_STAGE_DECALS_ACCUMULATE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_DECALS_ACCUMULATE_H_INCLUDED

#include <vostok/render/core/res_effect.h>

#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_decals_accumulate : public stage {
public:
	stage_decals_accumulate( renderer* in_renderer, renderer_context* context );

	virtual ~stage_decals_accumulate( );

	virtual void execute( ) override;
	virtual void execute_disabled( ) override;
	virtual void debug_render( ) override;

private:
	bool is_effects_ready( ) const;

private:
	res_effect_ptr	m_opaque_geometry_mask_effect;
	res_effect_ptr	m_apply_decal_effect;
};

STATIC_SIZE_ASSERT( stage_decals_accumulate, 0x18 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_DECALS_ACCUMULATE_H_INCLUDED
