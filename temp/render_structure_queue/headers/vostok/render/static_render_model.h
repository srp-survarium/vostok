////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_RENDER_MODEL_H_INCLUDED
#define RENDER_STATIC_RENDER_MODEL_H_INCLUDED

/* INCLUDES */
class vostok::render::render_model;

namespace vostok {
namespace render {

class static_render_model : public render_model {
public:
	inline		static_render_model	( ) { /* no source */ }
	virtual		~static_render_model( ) { /* no source */ }
}; // class static_render_model

STATIC_SIZE_ASSERT(static_render_model, 0x138);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_RENDER_MODEL_H_INCLUDED
