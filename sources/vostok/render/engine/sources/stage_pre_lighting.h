#ifndef VOSTOK_RENDER_ENGINE_STAGE_PRE_LIGHTING_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_PRE_LIGHTING_H_INCLUDED

#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;

class stage_pre_lighting : public stage {
public:
	stage_pre_lighting( renderer* in_renderer, renderer_context* in_context ) :
		stage( in_renderer, in_context )
	{
	}

	virtual ~stage_pre_lighting( ) { }

	virtual void execute( ) override;
};

STATIC_SIZE_ASSERT( stage_pre_lighting, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_PRE_LIGHTING_H_INCLUDED
