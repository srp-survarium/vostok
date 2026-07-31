#include "pch.h"
#include <vostok/render/core/sources/event_query.h>

namespace vostok {
namespace render {

// claude@NOTE: no legacy ancestor anywhere in the corpus (class is target-era;
// the held gpu_timer.h reference is declarations-only). Expected idiom is the
// D3D11_QUERY_EVENT sequence already ported in resource_manager.cpp
// begin_command_list/end_command_list [0x560970/0x560920]: init_query =
// CreateQuery(D3D11_QUERY_EVENT), issue = d3d_context->End(m_query), wait =
// spin on GetData(m_query,0,0,0)!=S_OK, release_query = m_query->Release().
// Matcher-phase work against the 0x5598xx bodies.
event_query::event_query( )
	: m_query( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559900]
}

event_query::~event_query( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559850]
}

void event_query::init_query( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5598d0]
}

void event_query::release_query( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559830]
}

void event_query::issue( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5598b0]
}

void event_query::wait( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559870]
}

} // namespace render
} // namespace vostok
