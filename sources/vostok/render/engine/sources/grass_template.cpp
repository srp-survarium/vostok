#include "pch.h"
#include "grass_template.h"
#include "grass_instance.h"

namespace vostok {
namespace render {

grass_template::grass_template( u32 in_index, grass_render_model_ptr const& in_model_ptr ) :
	m_render_model( in_model_ptr ),
	m_index( in_index ),
	m_sizes( 0.f, 0.f, 0.f )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7b1660]
}

grass_template::~grass_template( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7b15e0]
	for ( vector<grass_instance*>::iterator i = m_instances.begin( ); i != m_instances.end( ); ++i )
		DELETE( *i );
}

} // namespace render
} // namespace vostok
