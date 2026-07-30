#include "pch.h"
#include <vostok/render/facade/sources/game_renderer.h>

namespace vostok {
namespace render {
namespace game {

renderer::renderer( render::world& world, engine::world& engine_world )
	: m_world( world ),
	  m_render_engine_world( engine_world ),
	  m_debug( 0 ),
	  m_ui( 0 ),
	  m_scene( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x737620]
}

renderer::~renderer( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7375c0]
}

void renderer::goto_fullscreen( base_output_window_ptr const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7376c0]
}

void renderer::end_frame( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7379f0]
}

void renderer::draw_scene_impl( draw_scene_params const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x737d80]
}

void renderer::draw_scene(
	base_scene_ptr const&,
	base_scene_view_ptr const&,
	base_output_window_ptr const&,
	math::rectangle< float2 > const&,
	vostok::ui::font const*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x737e50]
}

debug::renderer& renderer::debug( ) const
{
	// FUNCTION BODY[0x737480]
	return *m_debug;
}

ui::renderer& renderer::ui( ) const
{
	// FUNCTION BODY[0x737470]
	return *m_ui;
}

scene_renderer& renderer::scene( ) const
{
	// FUNCTION BODY[0x737460]
	return *m_scene;
}

void renderer::show_movie( base_scene_view_ptr const&, survarium::flash_movie_resource_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x737860]
}

void renderer::hide_movie( base_scene_view_ptr const&, survarium::flash_movie_resource_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7376d0]
}

void renderer::show_text_manager( base_scene_view_ptr const&, survarium::flash_text_manager* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x737c40]
}

void renderer::hide_text_manager( base_scene_view_ptr const&, survarium::flash_text_manager* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x737b00]
}

void renderer::execute_scaleform_command( survarium::scaleform_render_command )
{
	// STATE[STUB]
	// FUNCTION BODY[0x737490]
}

} // namespace game
} // namespace render
} // namespace vostok
