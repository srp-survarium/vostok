#ifndef VOSTOK_RENDER_FACADE_SOURCES_ENGINE_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_ENGINE_RENDERER_H_INCLUDED

#include <vostok/render/api.h>

namespace vostok {
namespace render {

class world;

namespace engine {

class world;

class renderer : public core::noncopyable {
private:
	friend class render::world;
	explicit renderer( engine::world& engine_world );

public:
	void initialize( bool is_editor );
	u32 frame_id( );
	~renderer( ) { }

private:
	engine::world& m_render_engine_world;
};

} // namespace engine
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_ENGINE_RENDERER_H_INCLUDED
