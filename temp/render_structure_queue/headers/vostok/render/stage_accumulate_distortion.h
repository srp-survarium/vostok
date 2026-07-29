////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_ACCUMULATE_DISTORTION_H_INCLUDED
#define RENDER_STAGE_ACCUMULATE_DISTORTION_H_INCLUDED

/* INCLUDES */
class vostok::render::stage;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_accumulate_distortion : public stage {
public:
					stage_accumulate_distortion	( renderer* in_renderer, renderer_context* context );
	virtual			~stage_accumulate_distortion( );

	virtual	void	execute						( ) override;
}; // class stage_accumulate_distortion

STATIC_SIZE_ASSERT(stage_accumulate_distortion, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_ACCUMULATE_DISTORTION_H_INCLUDED
