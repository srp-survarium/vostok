// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_STAGE_APPLY_DISTORTION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_APPLY_DISTORTION_H_INCLUDED
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

class stage_apply_distortion : public stage {
public:
	stage_apply_distortion(
		renderer* in_renderer,
		renderer_context* context
	);

	virtual ~stage_apply_distortion( );
	virtual void execute( ) override;

	bool is_effects_ready( ) const;

private:
	res_effect_ptr m_sh_apply_distortion;
};

STATIC_SIZE_ASSERT( stage_apply_distortion, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_APPLY_DISTORTION_H_INCLUDED
