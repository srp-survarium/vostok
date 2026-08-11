#ifndef VOSTOK_RENDER_FACADE_SOURCES_ENGINE_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_ENGINE_RENDERER_H_INCLUDED

#include <vostok/render/api.h>

namespace vostok {
namespace render {

class world;

namespace engine {

class world;

class renderer : private core::noncopyable {
private:
	friend class render::world;
	explicit renderer( engine::world& engine_world );

public:
	void initialize( bool is_editor );
	u32 frame_id( );

private:
	engine::world& m_render_engine_world;
};

} // namespace engine

STATIC_SIZE_ASSERT( engine::renderer, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_ENGINE_RENDERER_H_INCLUDED
