// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/facade/sources/game_renderer.h>
#include <vostok/render/world.h>
#include <vostok/render/facade/sources/functor_command.h>
#include <vostok/render/facade/sources/functor_with_big_buffer_to_copy_command.h>
#include <vostok/render/facade/debug_renderer.h>
#include <vostok/render/facade/ui_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
namespace vostok {
namespace render {
namespace game {

renderer::renderer( render::world& world, engine::world& engine_world )
	: m_world( world ),
	  m_render_engine_world( engine_world )
{
	m_debug					= NEW( debug::renderer )(
		world.logic_channel(),
		*logic::g_allocator,
		engine_world
	);
	m_ui					= NEW( ui::renderer )(
		world.logic_channel(),
		*logic::g_allocator,
		engine_world
	);
	m_scene					= NEW( scene_renderer )(
		world.logic_channel(),
		*logic::g_allocator,
		engine_world,
		&m_debug->frustum
	);
}

renderer::~renderer( )
{
	DELETE	( m_scene );
	DELETE	( m_ui );
	DELETE	( m_debug );
}

void renderer::goto_fullscreen( base_output_window_ptr const& output_window )
{
	m_render_engine_world.goto_fullscreen	( output_window );
}

void renderer::end_frame( )
{
	m_world.logic_channel().owner_push_back(
		L_NEW( functor_command )(
			boost::bind( &render::world::end_frame_logic, &m_world )
		)
	);
}

void renderer::draw_scene_impl( draw_scene_params const& params )
{
	R_ASSERT	( params.scene );
	R_ASSERT	( params.scene_view );
	R_ASSERT	( params.render_output_window );
	if ( !params.render_output_window )
		return;

	m_render_engine_world.draw_scene(
		params.scene,
		params.scene_view,
		params.render_output_window,
		params.viewport,
		boost::bind(
			&one_way_render_channel::render_on_draw_scene,
			&m_world.logic_channel(),
			params.scene,
			params.scene_view,
			_1
		),
		params.default_font
	);
}

void renderer::draw_scene(
	base_scene_ptr const&			scene,
	base_scene_view_ptr const&		scene_view,
	base_output_window_ptr const&	render_output_window,
	math::rectangle< float2 > const&	viewport,
	vostok::ui::font const* const	default_font
)
{
	draw_scene_params params;
	params.scene					= scene;
	params.scene_view				= scene_view;
	params.render_output_window		= render_output_window;
	params.viewport					= viewport;
	params.default_font				= default_font;

	m_world.logic_channel().owner_push_back(
		L_NEW( functor_with_big_buffer_to_copy_command< draw_scene_params > )(
			boost::bind( &renderer::draw_scene_impl, this, _1 ),
			params
		)
	);
}

debug::renderer& renderer::debug( ) const
{
	ASSERT	( m_debug );
	return	*m_debug;
}

ui::renderer& renderer::ui( ) const
{
	ASSERT	( m_ui );
	return	*m_ui;
}

scene_renderer& renderer::scene( ) const
{
	ASSERT	( m_scene );
	return	*m_scene;
}

void renderer::show_movie( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie )
{
	m_world.logic_channel().owner_push_back(
		L_NEW( functor_command )(
			boost::bind(
				&engine::world::show_movie,
				&m_render_engine_world,
				scene_view,
				movie
			)
		)
	);
}

void renderer::hide_movie( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie )
{
	m_world.logic_channel().owner_push_back(
		L_NEW( functor_command )(
			boost::bind(
				&engine::world::hide_movie,
				&m_render_engine_world,
				scene_view,
				movie
			)
		)
	);
}

void renderer::show_text_manager( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* tm )
{
	m_world.logic_channel().owner_push_back(
		L_NEW( functor_command )(
			boost::bind(
				&engine::world::show_text_manager,
				&m_render_engine_world,
				scene_view,
				tm
			)
		)
	);
}

void renderer::hide_text_manager( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* tm )
{
	m_world.logic_channel().owner_push_back(
		L_NEW( functor_command )(
			boost::bind(
				&engine::world::hide_text_manager,
				&m_render_engine_world,
				scene_view,
				tm
			)
		)
	);
}

void renderer::execute_scaleform_command( survarium::scaleform_render_command command )
{
	m_world.logic_channel().owner_push_back(
		L_NEW( functor_command )(
			boost::bind(
				&engine::world::execute_scaleform_command,
				&m_render_engine_world,
				command
			)
		)
	);
}

} // namespace game
} // namespace render
} // namespace vostok
