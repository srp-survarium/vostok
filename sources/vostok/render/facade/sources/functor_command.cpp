#include "pch.h"
#include "functor_command.h"

namespace vostok {
namespace render {

functor_command::functor_command(
	on_execute_type const& on_execute,
	on_defer_execution_type const& on_defer_execution
)
	: base_command( !on_defer_execution.empty( ) ),
	  m_on_execute( on_execute ),
	  m_on_defer_execution( on_defer_execution )
{
	// FUNCTION BODY[0x771260]
	ASSERT				( on_execute );
}

void functor_command::execute( )
{
	// FUNCTION BODY[0x771300]
	m_on_execute		( );
}

void functor_command::defer_execution( )
{
	// FUNCTION BODY[0x7712f0]
	m_on_defer_execution( *this );
}

// claude@NOTE: no legacy ancestor - legacy functor_command.{h,cpp} defined only functor_command, not functor_command_with_notify; matcher-phase work.
// STATE[STUB]
functor_command_with_notify::~functor_command_with_notify( )
{
	// FUNCTION BODY[0x7712a0]
}

} // namespace render
} // namespace vostok
