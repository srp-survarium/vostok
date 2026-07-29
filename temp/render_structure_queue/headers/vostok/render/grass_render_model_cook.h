////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_RENDER_MODEL_COOK_H_INCLUDED
#define RENDER_GRASS_RENDER_MODEL_COOK_H_INCLUDED

/* INCLUDES */
struct vostok::render::render_model_cook;

/* FORWARD REFS */
class vostok::resources::query_result_for_cook;

namespace vostok {
namespace render {

struct grass_render_model_cook : public render_model_cook {
					grass_render_model_cook	( );

	virtual	void	translate_query			( resources::query_result_for_cook& parent ) override;

	virtual			~grass_render_model_cook( ) { /* no source */ }
}; // struct grass_render_model_cook

STATIC_SIZE_ASSERT(grass_render_model_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_RENDER_MODEL_COOK_H_INCLUDED
