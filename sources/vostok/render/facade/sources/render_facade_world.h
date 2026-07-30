#ifndef VOSTOK_RENDER_FACADE_SOURCES_RENDER_FACADE_WORLD_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_RENDER_FACADE_WORLD_H_INCLUDED

#include <vostok/render/api.h>
#include <vostok/render/facade/sources/one_way_render_channel.h>

namespace vostok {
namespace render {

namespace editor { class renderer; }
namespace engine { class renderer; class world; }
namespace game { class renderer; }

class VOSTOK_RENDER_API world {
public:
	world(
		memory::base_allocator& logic_allocator,
		memory::base_allocator* editor_allocator,
		configs::binary_config_ptr const& in_config,
		bool is_editor
	);
	~world( );

	void clear_resources( );
	void tick( );
	void enable_logic_thread_safety( bool value );

	engine::world& engine_world( ) { return *m_render_engine_world; }
	engine::renderer& engine_renderer( );
	game::renderer& game_renderer( );
	editor::renderer& editor_renderer( ) { return *m_editor_renderer; }

	one_way_render_channel& logic_channel( ) { return m_logic_channel; }
	void end_frame_logic( );

	one_way_render_channel& editor_channel( ) { return m_editor_channel; }
	void end_frame_editor( )
	{
		m_editor_channel.render_on_end_frame( );
		m_is_editor_frame_ended = true;
		if ( m_is_logic_frame_ended )
			end_frame( );
	}

private:
	world( world const& );
	world& operator=( world const& );
	void end_frame( );

private:
	one_way_render_channel m_logic_channel;
	one_way_render_channel m_editor_channel;
	engine::world* m_render_engine_world;
	engine::renderer* m_engine_renderer;
	game::renderer* m_game_renderer;
	editor::renderer* m_editor_renderer;
	volatile long m_is_logic_enabled;
	volatile long m_is_logic_frame_ended;
	bool m_is_editor_frame_ended;
	bool m_is_editor;
};

STATIC_SIZE_ASSERT( world, 0x190 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_RENDER_FACADE_WORLD_H_INCLUDED
