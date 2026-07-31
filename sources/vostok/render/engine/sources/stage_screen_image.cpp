#include "pch.h"
#include "stage_screen_image.h"

#include <vostok/math_float2.h>
#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/backend.h>

namespace vostok {
namespace render {

class scr_quad_effect : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& config
	) override;
};

STATIC_SIZE_ASSERT( scr_quad_effect, 0x4 );

struct screen_image_vertex {
	screen_image_vertex( float x, float y, float u, float v ) :
		position	( x, y ),
		text_coords	( u, v )
	{
	}

	float2 position;
	float2 text_coords;
};

STATIC_SIZE_ASSERT( screen_image_vertex, 0x10 );

void scr_quad_effect::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	// FUNCTION BODY[0x86360]
	VOSTOK_UNREFERENCED_PARAMETER	( config );

	compiler.begin_technique( /*LEVEL*/);
	compiler.begin_pass( "sa_quad", NULL, "image", shader_configuration(), NULL)
				.set_depth			( false, false, D3D_COMPARISON_ALWAYS)
				.set_alpha_blend	( false)
				.set_stencil		( false)
				//.def_sampler( "s_base", r2_rt_albedo)
			.end_pass()
		.end_technique();
}

stage_screen_image::stage_screen_image( renderer* in_renderer, renderer_context* in_context ) :
	stage( in_renderer, in_context )
{
// 	D3D_INPUT_ELEMENT_DESC	dcl[MAX_FVF_DECL_SIZE];
// 	CHECK_RESULT( D3DXDeclaratorFromFVF( D3DFVF_XYZRHW | D3DFVF_TEX1, dcl));
	D3D_INPUT_ELEMENT_DESC dcl[] =
	{
		{"POSITIONT",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 8, 	D3D_INPUT_PER_VERTEX_DATA, 0}
	};
	m_decl_ptr	= resource_manager::ref().create_declaration( dcl, array_size( dcl ));

	effect_manager::ref().create_effect<scr_quad_effect>(&m_present_effect);

	m_textures.resize(1, NULL);
}

bool stage_screen_image::is_effects_ready( ) const
{
	// FUNCTION BODY[0x70b8b0]
	return m_present_effect.c_ptr() != NULL;
}

void stage_screen_image::execute( res_texture_ptr t_image )
{
	// FUNCTION BODY[0x70b8c0]
	if (!is_effects_ready())
		return;

	if (!is_enabled())
	{
		execute_disabled();
		return;
	}

	backend::ref().reset_render_targets();
	backend::ref().set_depth_stencil_target( NULL);

	backend::ref().clear_render_targets( math::color( 0.f, 0.f, 0.f, 0.f));

	m_present_effect->apply( 0, 0 );

	ASSERT( m_textures.size() == 1);
	m_textures[0] = t_image;
//	backend::ref().set_ps_textures( NULL);      // To refuse caching of texture list.
	backend::ref().set_ps_texture( "t_base", &*t_image);
	//t_image->apply(0);

	backend::ref().set_declaration( &*m_decl_ptr);

	u32 base_offset;
	screen_image_vertex* vertices = (screen_image_vertex*)backend::ref().vertex.lock( 4, sizeof(screen_image_vertex), base_offset);

// 	vertices[1] = screen_image_vertex( x1-orig_x, y1-orig_y, 0, 0 );
// 	vertices[0] = screen_image_vertex( x2-orig_x, y1-orig_y, 1, 0 );
// 	vertices[3] = screen_image_vertex( x1-orig_x, y2-orig_y, 0, 1 );
// 	vertices[2] = screen_image_vertex( x2-orig_x, y2-orig_y, 1, 1 );

	vertices[0] = screen_image_vertex( -1,  1, 0, 0 );
	vertices[1] = screen_image_vertex(  1,  1, 1, 0 );
	vertices[2] = screen_image_vertex( -1, -1, 0, 1 );
	vertices[3] = screen_image_vertex(  1, -1, 1, 1 );

	backend::ref().vertex.unlock();

	backend::ref().set_vb( &backend::ref().vertex.buffer(), sizeof(screen_image_vertex));


// 	backend::ref().flush();
// 	device::ref().device()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertices, 6*4);

	backend::ref().render( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, base_offset);
}

} // namespace render
} // namespace vostok
