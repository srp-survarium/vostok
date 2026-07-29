////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_RENDER_MODEL_H_INCLUDED
#define RENDER_GRASS_RENDER_MODEL_H_INCLUDED

/* INCLUDES */
class vostok::render::grass_render_surface;
class vostok::render::render_model;

/* FORWARD REFS */
class vostok::render::model_lods_descriptor;
class vostok::render::render_surface;

namespace vostok {
namespace render {

class grass_render_model : public render_model {
public:
	inline			grass_render_model	( ) { /* no source */ }

	virtual	void	set_children		( render_surface** children, u8 count, model_lods_descriptor* lods ) override;

	virtual			~grass_render_model	( ) { /* no source */ }

	/* 0x0000 */	/* render_model */
	/* 0x0138 */	grass_render_surface*		m_l0;
	/* 0x013c */	grass_render_surface*		m_l1;
	/* 0x0140 */	grass_render_surface*		m_l2;
}; // class grass_render_model

STATIC_SIZE_ASSERT(grass_render_model, 0x148);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_RENDER_MODEL_H_INCLUDED
