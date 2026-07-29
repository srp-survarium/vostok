#include "pch.h"
#include "stage.h"

namespace vostok {
namespace render {

// STATE[STUB]
stage::stage( renderer* in_renderer, renderer_context* in_context ) :
	m_context		( in_context ),
	m_renderer		( in_renderer ),
	m_enabled		( true ),
	m_prev_enabled	( true )
{
	// FUNCTION BODY[0x62fe90]
}

// STATE[STUB]
void stage::execute( )
{
	// FUNCTION BODY[0x62fe80]
}

// STATE[STUB]
void stage::set_enabled( bool enable )
{
	// FUNCTION BODY[0x62fe70]
	m_enabled = enable;
}

// STATE[STUB]
bool stage::is_enabled( ) const
{
	// FUNCTION BODY[0x62fe60]
	return m_enabled;
}

} // namespace render
} // namespace vostok
