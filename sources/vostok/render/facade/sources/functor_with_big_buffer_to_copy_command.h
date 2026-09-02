// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <vostok/render/facade/base_command.h>

namespace vostok {
namespace render {

template < typename T >
class functor_with_big_buffer_to_copy_command :
	public base_command,
	private boost::noncopyable
{
public:
	typedef boost::function< void ( T const& ) > on_execute_type;
	typedef boost::function< void ( base_command& ) > on_defer_execution_type;

	inline functor_with_big_buffer_to_copy_command(
		on_execute_type const& on_execute,
		T const& data,
		on_defer_execution_type const& on_defer_execution = on_defer_execution_type( )
	);

	virtual void execute( );
	virtual void defer_execution( );

private:
	on_execute_type m_on_execute;
	on_defer_execution_type m_on_defer_execution;
	T const m_data;
};

} // namespace render
} // namespace vostok

#include <vostok/render/facade/sources/functor_with_big_buffer_to_copy_command_inline.h>

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED
