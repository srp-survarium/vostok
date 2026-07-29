////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\facade\sources\functor_command.h"

namespace vostok {
namespace render {

// STATE[STUB]
 functor_command::functor_command(
	boost::function< void() > const&	on_execute,
	boost::function< void( base_command& ) > const&	on_defer_execution
)
{
	// FUNCTION BODY[0x771260]: 1
	// <0>
	// ******
}

// STATE[STUB]
void functor_command::execute( )
{
	// FUNCTION BODY[0x771300]: 0
	// ******
}

// STATE[STUB]
void functor_command::defer_execution( )
{
	// FUNCTION BODY[0x7712f0]: 1
	// <0x7712f0>|0x000|+0x009:'31'
	// ******
}

// STATE[STUB]
 functor_command_with_notify::~functor_command_with_notify( )
{
	// FUNCTION BODY[0x7712a0]: 1
	// <0x7712a4>|0x004|+0x013:'36'
	// ******
}

} // namespace render
} // namespace vostok
