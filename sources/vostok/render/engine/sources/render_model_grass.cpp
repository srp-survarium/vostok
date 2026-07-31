#include "pch.h"
#include "render_model_grass.h"

namespace vostok {
namespace render {

grass_render_surface::grass_render_surface( ) :
	m_vertices( 0 ),
	m_num_vertices( 0 ),
	m_indices( 0 ),
	m_num_indices( 0 )
{
	// FUNCTION BODY[0x779890]
	m_vertex_input_type = grassmesh_vertex_input_type;
}

// STATE[STUB]
// claude@NOTE: legacy body needs D3DXGetDeclLength/D3DXGetDeclVertexSize (d3dx9mesh) and
// predates the CPU-side m_vertices/m_indices copies - skipped; seed kept in temp/render_legacy.
void grass_render_surface::load( configs::binary_config_value const&, memory::chunk_reader& )
{
	// FUNCTION BODY[0x779670]
}

grass_render_surface::~grass_render_surface( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x779830]
}

void grass_render_model::set_children( render_surface**, u8, model_lods_descriptor* )
{
	// claude@NOTE: no legacy ancestor - legacy append_surface is declaration-only and a retired one-surface contract regardless; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x7795f0]
}

} // namespace render
} // namespace vostok
