// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "game_generate_shaders.h"
#include <vostok/render/world.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/console_command_processor.h>
#include <vostok/console_command.h>
#include <vostok/resources.h>
#include <vostok/debug/debug.h>

namespace survarium {

generate_shaders_world::generate_shaders_world( render::world& render_world ) :
	m_renderer( render_world.game_renderer() ),
	m_first_call_reset_renderer( false )
{
}

void generate_shaders_world::generate_renderer_shaders( )
{


	console_commands::console_command* antialiasing_method_command	= console_commands::find( "r_antialiasing_method" );
	console_commands::console_command* shadow_quality_command		= console_commands::find( "r_shadow_quality" );
	console_commands::console_command* lighting_quality_command		= console_commands::find( "r_lighting_quality" );
	console_commands::console_command* shading_quality_command		= console_commands::find( "r_shading_quality" );
	console_commands::console_command* post_process_quality_command	= console_commands::find( "r_post_process_quality" );

	u32& antialiasing_method_command_value	= static_cast< console_commands::cc_u32* >( antialiasing_method_command )->get_value();
	u32& shadow_quality_command_value		= static_cast< console_commands::cc_u32* >( shadow_quality_command )->get_value();
	u32& lighting_quality_command_value		= static_cast< console_commands::cc_u32* >( lighting_quality_command )->get_value();
	u32& shading_quality_command_value		= static_cast< console_commands::cc_u32* >( shading_quality_command )->get_value();
	u32& post_process_quality_command_value	= static_cast< console_commands::cc_u32* >( post_process_quality_command )->get_value();

	antialiasing_method_command_value		= ( u32 )-1;
	shadow_quality_command_value			= ( u32 )-1;
	shading_quality_command_value			= ( u32 )-1;
	post_process_quality_command_value		= ( u32 )-1;
	lighting_quality_command_value			= ( u32 )-1;


	u32 antialiasing_method_values[ 3 ]		= { 0, 1, 2 };
	u32 shadow_quality_values[ 2 ]			= { 0, 3 };
	u32 shading_quality_values[ 2 ]			= { 0, 3 };
	u32 lighting_quality_values[ 2 ]		= { 0, 3 };
	u32 post_process_quality_values[ 2 ]	= { 0, 3 };

	for ( u32 antialiasing_method = 0; antialiasing_method < 3; ++antialiasing_method )
		for ( u32 shadow_quality = 0; shadow_quality < 2; ++shadow_quality )
			for ( u32 lighting_quality = 0; lighting_quality < 2; ++lighting_quality )
				for ( u32 shading_quality = 0; shading_quality < 2; ++shading_quality )
					for ( u32 post_process_quality = 0; post_process_quality < 2; ++post_process_quality )
					{
						volatile long waiting_for = 1;

						m_renderer.scene().begin_render_options_changing( &waiting_for );

						while ( waiting_for )
						{
							resources::dispatch_callbacks();
							threading::yield( 0 );
						}

						antialiasing_method_command_value	= antialiasing_method_values[ antialiasing_method ];
						shadow_quality_command_value			= shadow_quality_values[ shadow_quality ];
						lighting_quality_command_value		= lighting_quality_values[ lighting_quality ];
						shading_quality_command_value		= shading_quality_values[ shading_quality ];
						post_process_quality_command_value	= post_process_quality_values[ post_process_quality ];
						m_renderer.scene().end_render_options_changing(
							render::scene_ptr(), render::render_output_window_ptr(), false, false, &waiting_for
						);

						while ( waiting_for || resources::pending_queries_count() )
						{
							resources::dispatch_callbacks();
							threading::yield( 0 );
						}

						u32 const pending_queries_count = resources::pending_queries_count();
						LOG_ERROR( "pending_queries_count:%d", pending_queries_count );
					}
}

void generate_shaders_world::generate_materials_shaders( )
{
	console_commands::console_command* shadow_quality_command	= console_commands::find( "r_shadow_quality" );
	console_commands::console_command* shading_quality_command	= console_commands::find( "r_shading_quality" );

	u32& shadow_quality_command_value	= static_cast< console_commands::cc_u32* >( shadow_quality_command )->get_value();
	u32& shading_quality_command_value	= static_cast< console_commands::cc_u32* >( shading_quality_command )->get_value();

	shadow_quality_command_value		= ( u32 )-1;
	shading_quality_command_value		= ( u32 )-1;

	u32 shadow_quality_values[ 2 ]		= { 0, 3 };
	u32 shading_quality_values[ 2 ]		= { 0, 3 };


	for ( u32 shadow_quality = 0; shadow_quality < 2; ++shadow_quality )
		for ( u32 shading_quality = 0; shading_quality < 2; ++shading_quality )
		{
			volatile long waiting_for = 1;

			m_renderer.scene().begin_render_options_changing( &waiting_for );

			while ( waiting_for )
			{
				resources::dispatch_callbacks();
				threading::yield( 0 );
			}

			shadow_quality_command_value		= shadow_quality_values[ shadow_quality ];
			shading_quality_command_value	= shading_quality_values[ shading_quality ];
			m_renderer.scene().end_render_options_changing(
				render::scene_ptr(), render::render_output_window_ptr(), true, false, &waiting_for
			);

			while ( waiting_for || resources::pending_queries_count() )
			{
				resources::dispatch_callbacks();
				threading::yield( 0 );
			}

			u32 const pending_queries_count = resources::pending_queries_count();
			LOG_ERROR( "pending_queries_count:%d", pending_queries_count );
		}
}
void generate_shaders_world::tick( u32 current_frame_id )
{

	if ( !m_first_call_reset_renderer )
	{
		m_renderer.scene().reset_renderer();

		generate_renderer_shaders();
		generate_materials_shaders();

		m_first_call_reset_renderer = true;
	}

	static u32 tick_id;

	if ( tick_id % 100 == 0 )
	{
		u32 const pending_queries_count = resources::pending_queries_count();
		LOG_ERROR( "pending_queries_count:%d", pending_queries_count );
	}

	debug::debug_message_box( "shaders generated" );
	debug::terminate( "" );

	++tick_id;

	m_renderer.end_frame();
}

} // namespace survarium
