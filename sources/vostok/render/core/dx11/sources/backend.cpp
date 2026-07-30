#include "pch.h"
#include <vostok/render/core/backend.h>

namespace vostok {
namespace render {

backend::backend( ) :
	vertex( 1536 * 1024 ),
	index( 512 * 1024 ),
	m_vs_textures_handler( m_vs ),
	m_vs_samplers_handler( m_vs ),
	m_gs_textures_handler( m_gs ),
	m_gs_samplers_handler( m_gs ),
	m_ps_textures_handler( m_ps ),
	m_ps_samplers_handler( m_ps )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560260]
}

backend::~backend( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55fa10]
}

void backend::clear_depth_stencil( u32, float, u8 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f5b0]
}

void backend::clear_render_targets( float, float, float, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f530]
}

void backend::clear_render_targets( math::color )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f860]
}

void backend::clear_render_targets(
	math::color,
	math::color,
	math::color,
	math::color
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f5f0]
}

void backend::on_device_destroy( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f510]
}

bool sorted_vector_predicate( shader_constant_host const*, shared_string const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f4f0]
	return false;
}

shader_constant_host* backend::register_constant_host( shared_string const&, enum_constant_type )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f930]
	return 0;
}

void backend::update_input_layout( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f9b0]
}

void backend::set_render_target( enum_render_target_enum, render_target const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f4c0]
}

void start_profiling( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f4b0]
}

double end_profiling( pcstr, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f4a0]
	return 0.0;
}

void backend::flush( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55fd70]
}

void backend::render_indexed( D3D_PRIMITIVE_TOPOLOGY, u32, u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5601b0]
}

void backend::render( D3D_PRIMITIVE_TOPOLOGY, u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560130]
}

} // namespace render
} // namespace vostok
