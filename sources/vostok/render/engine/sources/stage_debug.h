#ifndef VOSTOK_RENDER_ENGINE_STAGE_DEBUG_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_DEBUG_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>

#include "sphere_geometry.h"
#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;
class shader_constant_host;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_debug : public stage {
public:
	stage_debug( renderer* in_renderer, renderer_context* in_context ) :
		stage							( in_renderer, in_context ),
		m_sphere_geometry				( 16, 16 ),
		m_preview_mip_index_parameter	( 0 )
	{
		// STATE[STUB]
	}

	virtual ~stage_debug( );
	virtual void execute( ) override;

	void render_environment_probe_preview( );
	bool is_effects_ready( ) const;

private:
	res_effect_ptr			m_debug_environment_probe_preview_effect;
	sphere_geometry			m_sphere_geometry;
	shader_constant_host*	m_preview_mip_index_parameter;
};

STATIC_SIZE_ASSERT( stage_debug, 0x2C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_DEBUG_H_INCLUDED
