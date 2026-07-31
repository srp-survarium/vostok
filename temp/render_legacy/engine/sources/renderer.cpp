////////////////////////////////////////////////////////////////////////////
//	Created		: 10.02.2009
//	Author		: Igor Lobanchikov
//	Copyright ( color) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER (2026-07-31): ported to
// sources/vostok/render/engine/sources/renderer.cpp: is_effects_ready, ctor,
// dtor, setup_render_output_window, toggle_render_stage, execute_stages,
// render, present (+ the s_disabled_shader_constansts_set cc and the
// `static statistics m_statistics;` singleton instance moved with them).
// Kept below:
// - s_view_mode cc: registered-only in legacy; no ported body reads it -
//   candidate for the stage_view_mode/statistics harvest to reconcile.
// - fill_surface: lives in stage_gbuffer.cpp in the shipped exe
//   (FUNCTION BODY[0x62cec0] stub there) - NOT this TU's function; port it
//   with the stage_gbuffer batch.
// - renderer::draw_text (member): canonical has free draw_text/draw_text_shadowed
//   with different signatures and real target bodies; this legacy member's body
//   is entirely commented out - nothing portable.

namespace vostok {
namespace render {

static u32 s_view_mode_value = u32(lit_view_mode);
static vostok::console_commands::cc_u32 s_view_mode("render_view_mode",
												   s_view_mode_value,
												   0,
												   num_view_modes,
												   true,
												   vostok::console_commands::command_type_engine_internal);

#ifndef MASTER_GOLD
// TODO: Make as a general functions.
static void fill_surface(ref_rt surf, renderer_context* context)
{
	float w = float(surf->width());
	float h = float(surf->height());
	float z = 0.0f;

	backend::ref().set_render_targets( &*surf, 0, 0, 0);
	backend::ref().reset_depth_stencil_target();
	backend::ref().clear_render_targets( vostok::math::color( 1.0f, 1.0f, 1.0f, 0.0f));

	float2	p0( 0,0);
	float2	p1( 1,1);

	u32		offset;

	float3 const* eye_rays = context->get_eye_rays();

	// Fill vertex buffer
	vertex_formats::Tquad* pv = backend::ref().vertex.lock<vertex_formats::Tquad>( 4, offset);
	pv->set( 0, h, z, 1.0, eye_rays[1].x, eye_rays[1].y, eye_rays[1].z, p0.x, p1.y); pv++;
	pv->set( 0, 0, z, 1.0, eye_rays[0].x, eye_rays[0].y, eye_rays[0].z, p0.x, p0.y); pv++;
	pv->set( w, h, z, 1.0, eye_rays[3].x, eye_rays[3].y, eye_rays[3].z, p1.x, p1.y); pv++;
	pv->set( w, 0, z, 1.0, eye_rays[2].x, eye_rays[2].y, eye_rays[2].z, p1.x, p0.y); pv++;
	backend::ref().vertex.unlock();

	context->m_g_quad_eye_ray->apply();

	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);
}
#endif // #ifndef MASTER_GOLD

void renderer::draw_text( pcstr text, vostok::math::float2 const& position, vostok::ui::font* const in_font, vostok::math::color const& in_color )
{
	VOSTOK_UNREFERENCED_PARAMETERS(text, &position, in_font, &in_color);
	// TODO:
// 	vectora<vostok::render::ui::vertex>	ui_vertices(g_allocator);
//
// 	u32 symb_count			= strlen(text);
// 	u32 max_verts			= symb_count*4; // use TriangleList+ IndexBuffer(0-1-2, 3-2-1)
//
// 	pcstr ch				= text;
// 	float const height		= in_font->get_height();
// 	float height_ts			= in_font->get_height_ts();
//
// 	float curr_word_len		= 0.0f;
// 	pcstr next_word			= NULL;
//
// 	//if(m_mode==tm_multiline)
// 	in_font->parse_word			(ch, curr_word_len, next_word);
//
//
// 	//vostok::render::ui::command* cmd = render.create_command(max_verts, prim_type, point_type);
// 	for(u32 i=0; i<symb_count; ++i, ++ch)
// 	{
// 		u32 clr = in_color;//(i>=(u32)idx1 && i<(u32)idx2)?sel_clr:in_color;
//
// 		float3 uv			= in_font->get_char_tc_ts(*ch);
// 		float3 const& tc	= in_font->get_char_tc(*ch);
//
// 		//if((m_mode==tm_multiline) && (ch==next_word))
// 		//{
// 		//	parse_word		(ch, m_font, curr_word_len, next_word);
// 		//	if(pos_rt.x+curr_word_len >get_size().x)
// 		//	{
// 		//		pos_rt.x		= 0.0f;
// 		//		pos_rt.y		+= height;
// 		//	}
// 		//}
//
// 		cmd->push_point		(pos.x+pos_rt.x,		pos.y+pos_rt.y+height,	0.0f, clr, uv.x,		uv.y+height_ts);
// 		cmd->push_point		(pos.x+pos_rt.x,		pos.y+pos_rt.y,			0.0f, clr, uv.x,		uv.y);
// 		cmd->push_point		(pos.x+pos_rt.x+tc.z,	pos.y+pos_rt.y+height,	0.0f, clr, uv.x+uv.z,	uv.y+height_ts);
// 		cmd->push_point		(pos.x+pos_rt.x+tc.z,	pos.y+pos_rt.y,			0.0f, clr, uv.x+uv.z,	uv.y);
//
// 		pos_rt.x			+= tc.z;
// 	}
// //	cmd->set_shader();
// //	cmd->set_geom();
// //	w.ui().destroy_command(cmd);
// 	render.push_command		(cmd);
}

//flash_renderer& renderer::get_flash_renderer( )
//{
//	return *m_flash_renderer;
//}

} // namespace render
} // namespace vostok
