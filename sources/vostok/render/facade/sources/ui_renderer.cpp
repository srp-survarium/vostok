// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/facade/sources/ui_renderer.h>
#include <vostok/render/world.h>
#include <vostok/render/engine/world.h>
#include <vostok/render/facade/sources/ui_draw_vertices_command.h>
namespace vostok {
namespace render {

ui::renderer::renderer(
	one_way_render_channel&		channel,
	memory::base_allocator&		allocator,
	engine::world&				engine_world
)
	: m_channel( channel ),
	  m_render_engine_world( engine_world ),
	  m_allocator( allocator )
{
}

void ui::renderer::draw_vertices(
	base_scene_view_ptr const&		scene_view,
	ui::vertex const* const			begin,
	ui::vertex const* const			end,
	const u32						primitives_type,
	const u32						points_type
)
{
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, ui::draw_vertices_command ) ( m_render_engine_world, scene_view, begin, end, m_allocator, primitives_type, points_type ) );
}

void ui::renderer::draw_text(
	base_scene_view_ptr const&		scene_view,
	pcstr const&					text,
	vostok::ui::font const&			font,
	float2 const&					position,
	math::color const&				text_color,
	math::color const&				selection_color,
	const u32						max_line_size,
	const bool						is_multiline,
	const u32						start_selection,
	const u32						end_selection
)
{
	vectora< ui::vertex > vertices( m_allocator );
	vertices.resize				( strings::length( text ) * 4 );
	m_render_engine_world.draw_text	(
		vertices,
		text,
		font,
		position,
		text_color,
		selection_color,
		max_line_size,
		is_multiline,
		start_selection,
		end_selection
	);

	u32 const primitives_type	= 0;
	u32 const points_type		= 0;
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, ui::draw_vertices_command ) ( m_render_engine_world, scene_view, &*vertices.begin(), &*vertices.end(), m_allocator, primitives_type, points_type ) );
}

} // namespace render
} // namespace vostok
