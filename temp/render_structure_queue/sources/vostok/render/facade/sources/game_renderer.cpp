////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\facade\sources\game_renderer.h"

namespace vostok {
namespace render {

// STATE[STUB]
 game::renderer::renderer( world& world, engine::world& engine_world )
{
	// FUNCTION BODY[0x737620]: 3
	// <0x73762b>|0x00b|+0x02d:'27'
	// <0x737658>|0x038|+0x026:'28'
	// <0x73767e>|0x05e|+0x02c:'29'
	// <0x7376aa>|0x08a|-0x007:'29'
	// <0x7376a3>|0x083|+0x00c:'30'
	// ******
}

// STATE[STUB]
 game::renderer::~renderer( )
{
	// FUNCTION BODY[0x7375c0]: 3
	// <0x7375c0>|0x000|+0x025:'34'
	// <0x7375e5>|0x025|+0x01b:'35'
	// <0x737600>|0x040|+0x017:'36'
	// ******
}

// STATE[STUB]
void game::renderer::goto_fullscreen( base_output_window_ptr const& output_window )
{
	// FUNCTION BODY[0x7376c0]: 1
	// <0x7376c0>|0x000|+0x00f:'50'
	// ******
}

// STATE[STUB]
void game::renderer::end_frame( )
{
	// CALL SITE INFO
	// <0x737abf> -> < unknown >
	// ******

	// FUNCTION BODY[0x7379f0]: 1
	// <0x7379f3>|0x003|+0x100:'55'
	// ******
}

// STATE[STUB]
void game::renderer::draw_scene_impl( game::renderer::draw_scene_params const& params )
{
	// CALL SITE INFO
	// <0x737e3f> -> < unknown >
	// ******

	// FUNCTION BODY[0x737d80]: 15
	// <0x737d8c>|0x00c|+0x014:'72'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x737da0>|0x020|+0x0a4:'86'
	// ******
}

// STATE[STUB]
void game::renderer::draw_scene(
	base_scene_ptr const&				scene,
	base_scene_view_ptr const&			scene_view,
	base_output_window_ptr const&		render_output_window,
	math::rectangle< float2 > const&	viewport,
	ui::font const*						default_font
)
{
	// LOCALS
	// game::renderer::draw_scene_params params
	// ******

	// CALL SITE INFO
	// <0x737f5e> -> < unknown >
	// <0x737f8d> -> < unknown >
	// ******

	// FUNCTION BODY[0x737e50]: 17
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <0x737e5a>|0x00a|+0x138:'113'
	// ******
}

// STATE[STUB]
debug::renderer& game::renderer::debug( ) const
{
	// FUNCTION BODY[0x737480]: 2
	// <0>
	// <0x737480>|0x000|+0x003:'119'
	// ******
}

// STATE[STUB]
ui::renderer& game::renderer::ui( ) const
{
	// FUNCTION BODY[0x737470]: 2
	// <0>
	// <0x737470>|0x000|+0x003:'125'
	// ******
}

// STATE[STUB]
scene_renderer& game::renderer::scene( ) const
{
	// FUNCTION BODY[0x737460]: 2
	// <0>
	// <0x737460>|0x000|+0x003:'131'
	// ******
}

// STATE[STUB]
void game::renderer::show_movie( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie )
{
	// CALL SITE INFO
	// <0x737983> -> < unknown >
	// ******

	// FUNCTION BODY[0x737860]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x737863>|0x003|+0x152:'140'
	// ******
}

// STATE[STUB]
void game::renderer::hide_movie( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie )
{
	// CALL SITE INFO
	// <0x7377f7> -> < unknown >
	// ******

	// FUNCTION BODY[0x7376d0]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7376d9>|0x009|+0x150:'148'
	// ******
}

// STATE[STUB]
void game::renderer::show_text_manager( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* tm )
{
	// CALL SITE INFO
	// <0x737d3b> -> < unknown >
	// ******

	// FUNCTION BODY[0x737c40]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x737c43>|0x003|+0x12a:'157'
	// ******
}

// STATE[STUB]
void game::renderer::hide_text_manager( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* tm )
{
	// CALL SITE INFO
	// <0x737c02> -> < unknown >
	// ******

	// FUNCTION BODY[0x737b00]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x737b09>|0x009|+0x12b:'165'
	// ******
}

// STATE[STUB]
void game::renderer::execute_scaleform_command( survarium::scaleform_render_command command )
{
	// CALL SITE INFO
	// <0x73757e> -> < unknown >
	// ******

	// FUNCTION BODY[0x737490]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x737499>|0x009|+0x117:'174'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	boost::function< void( vostok::render::game::renderer::draw_scene_params const& ) >
	// 	on_execute_type;

	// ******

} // namespace render
} // namespace vostok
