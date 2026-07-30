#ifndef VOSTOK_RENDER_ENGINE_STAGE_ACCUMULATE_DISTORTION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_ACCUMULATE_DISTORTION_H_INCLUDED

#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;

class stage_accumulate_distortion : public stage {
public:
	stage_accumulate_distortion(
		renderer* in_renderer,
		renderer_context* context
	);

	virtual ~stage_accumulate_distortion( );
	virtual void execute( ) override;
};

STATIC_SIZE_ASSERT( stage_accumulate_distortion, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_ACCUMULATE_DISTORTION_H_INCLUDED
