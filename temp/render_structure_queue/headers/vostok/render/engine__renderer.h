////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENGINE_RENDERER_H_INCLUDED
#define RENDER_ENGINE_RENDERER_H_INCLUDED

/* INCLUDES */
class vostok::core::noncopyable;
class vostok::render::engine::world;

namespace vostok {
namespace render {

class engine::renderer : public core::noncopyable {
			explicit	renderer	( engine::world& engine_world );

public:
			void		initialize	( bool is_editor );

			u32			frame_id	( );

	inline				~renderer	( ) { /* no source */ }

	/* 0x0000 */	/* core::noncopyable */
private:
	/* 0x0000 */	engine::world&		m_render_engine_world;
}; // class engine::renderer

STATIC_SIZE_ASSERT(engine::renderer, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENGINE_RENDERER_H_INCLUDED
