#include "pch.h"
#include "stage_screen_image.h"

#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/backend.h>

#include "scr_quad_effect.h"
#include "screen_image_vertex.h"

namespace vostok {
namespace render {

void scr_quad_effect::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( config );

	compiler.begin_technique( );
	compiler.begin_pass( "sa_quad", NULL, "present", shader_configuration( ), NULL );
	compiler.set_depth( false, false, D3D_COMPARISON_ALWAYS );
	compiler.set_alpha_blend( false );
	compiler.set_stencil( false );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "sa_quad", NULL, "present_2rt", shader_configuration( ), NULL );
	compiler.set_depth( false, false, D3D_COMPARISON_ALWAYS );
	compiler.set_alpha_blend( false );
	compiler.set_stencil( false );
	compiler.end_pass( );
	compiler.end_technique( );
}

stage_screen_image::stage_screen_image( renderer* in_renderer, renderer_context* context ) :
	stage( in_renderer, context )
{
	D3D_INPUT_ELEMENT_DESC dcl[] =
	{
		{ "POSITIONT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_decl_ptr = resource_manager::ref( ).create_declaration( dcl, array_size( dcl ) );

	effect_manager::ref( ).create_effect< scr_quad_effect >( &m_present_effect );

	m_textures.resize( 1, NULL );
}

bool stage_screen_image::is_effects_ready( ) const
{
	return m_present_effect.c_ptr( ) != NULL;
}

void stage_screen_image::execute( res_texture_ptr t_image )
{
	if ( !is_effects_ready( ) )
		return;

	if ( !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}
	// 3 target lines are likely retail-compiled-out source.
	backend::ref( ).reset_render_targets( );
	backend::ref( ).set_depth_stencil_target( NULL );
	// 4 target lines are likely retail-compiled-out source.
	backend::ref( ).clear_render_targets( math::color( 0.5f, 0.5f, 0.5f, 0.5f ) );
	// 4 target lines are likely retail-compiled-out source.
	m_present_effect->apply( 0, 0 );

	ASSERT( m_textures.size( ) == 1 );
	m_textures[0] = t_image;
	backend::ref( ).set_ps_texture( "t_base", &*t_image );

	backend::ref( ).set_declaration( &*m_decl_ptr );

	u32 base_offset;
	screen_image_vertex* vertices = (screen_image_vertex*)backend::ref( ).vertex.lock( 4, sizeof(screen_image_vertex), base_offset );

	vertices[0] = screen_image_vertex( -1,  1, 0, 0 );
	vertices[1] = screen_image_vertex(  1,  1, 1, 0 );
	vertices[2] = screen_image_vertex( -1, -1, 0, 1 );
	vertices[3] = screen_image_vertex(  1, -1, 1, 1 );

	backend::ref( ).vertex.unlock( );

	backend::ref( ).set_vb( &backend::ref( ).vertex.buffer( ), sizeof(screen_image_vertex) );

	backend::ref( ).render( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, base_offset );
}

} // namespace render
} // namespace vostok
