#ifndef VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_INLINE_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_INLINE_H_INCLUDED

namespace vostok {
namespace render {

template < typename T >
inline functor_with_big_buffer_to_copy_command< T >::functor_with_big_buffer_to_copy_command(
	on_execute_type const& on_execute,
	T const& data,
	on_defer_execution_type const& on_defer_execution
)	:
	base_command		( !on_defer_execution.empty( ) ),
	m_on_execute		( on_execute ),
	m_on_defer_execution( on_defer_execution ),
	m_data				( data )
{
	R_ASSERT			( on_execute );
}

template < typename T >
inline void functor_with_big_buffer_to_copy_command< T >::execute( )
{
	m_on_execute		( m_data );
}

template < typename T >
inline void functor_with_big_buffer_to_copy_command< T >::defer_execution( )
{
	m_on_defer_execution( *this );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_INLINE_H_INCLUDED
