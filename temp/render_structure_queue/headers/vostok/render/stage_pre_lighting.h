////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_PRE_LIGHTING_H_INCLUDED
#define RENDER_STAGE_PRE_LIGHTING_H_INCLUDED

/* INCLUDES */
class vostok::render::stage;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_pre_lighting : public stage {
public:
	inline			stage_pre_lighting	( renderer* arg_0, renderer_context* arg_1 ) { /* no source */ }

	virtual	void	execute				( ) override;

	virtual			~stage_pre_lighting	( ) { /* no source */ }
}; // class stage_pre_lighting

STATIC_SIZE_ASSERT(stage_pre_lighting, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_PRE_LIGHTING_H_INCLUDED
