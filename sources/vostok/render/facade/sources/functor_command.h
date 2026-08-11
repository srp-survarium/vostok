#ifndef VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_COMMAND_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_COMMAND_H_INCLUDED

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <vostok/render/facade/base_command.h>

namespace vostok {
namespace render {

class functor_command : public base_command, private boost::noncopyable {
public:
	typedef boost::function< void ( ) > on_execute_type;
	typedef boost::function< void ( base_command& ) > on_defer_execution_type;

	functor_command(
		on_execute_type const& on_execute,
		on_defer_execution_type const& on_defer_execution = on_defer_execution_type( )
	);

	virtual void execute( );
	virtual void defer_execution( );
	virtual ~functor_command( ) { /* no source */ }

private:
	on_execute_type m_on_execute;
	on_defer_execution_type m_on_defer_execution;
};

STATIC_SIZE_ASSERT( functor_command, 0x98 );

class functor_command_with_notify : public functor_command {
typedef functor_command super;
typedef boost::function< void ( ) > on_destroy_type;

public:
	virtual ~functor_command_with_notify( );

	inline functor_command_with_notify(
		super::on_execute_type const& on_execute,
		on_destroy_type const& on_destroy
	)
		: super( on_execute ),
		  m_on_destroy( on_destroy )
	{
	}

private:
	on_destroy_type m_on_destroy;
};

STATIC_SIZE_ASSERT( functor_command_with_notify, 0xb8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_COMMAND_H_INCLUDED
