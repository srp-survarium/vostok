#include "pch.h"
#include "functor_command.h"

namespace vostok {
namespace render {

// STATE[STUB]
functor_command::functor_command(
	on_execute_type const& on_execute,
	on_defer_execution_type const& on_defer_execution
)
	: base_command( !on_defer_execution.empty( ) ),
	  m_on_execute( on_execute ),
	  m_on_defer_execution( on_defer_execution )
{
	// FUNCTION BODY[0x771260]
}

// STATE[STUB]
void functor_command::execute( )
{
	// FUNCTION BODY[0x771300]
}

// STATE[STUB]
void functor_command::defer_execution( )
{
	// FUNCTION BODY[0x7712f0]
}

// STATE[STUB]
functor_command_with_notify::~functor_command_with_notify( )
{
	// FUNCTION BODY[0x7712a0]
}

} // namespace render
} // namespace vostok
