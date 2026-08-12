#ifndef VOSTOK_RENDER_ENGINE_STATISTICS_INLINE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STATISTICS_INLINE_H_INCLUDED

#include <vostok/c_array_functions.h>
#include <vostok/math_functions.h>

namespace vostok {
namespace render {

template < typename T >
inline statistics_value< T >::statistics_value( statistics_group* group, pcstr name ) :
	statistics_base				( group, name ),
	value						( T( ) ),
	min_value					( T( 1000 ) ),
	max_value					( T( ) ),
	value_num_max_digits		( 0 ),
	min_value_num_max_digits	( 0 ),
	max_value_num_max_digits	( 0 ),
	min_value_temp				( T( 1000 ) ),
	max_value_temp				( T( ) ),
	history_index				( 0 ),
	min_max_frame_index			( 0 )
{
	history[0]					= T( );
}

template < typename T >
inline void statistics_value< T >::start( )
{
	history[history_index++]	= value;

	if ( history_index == array_size( history ) )
		history_index			= 0;

	if ( min_max_frame_index > 32 )
	{
		min_value				= min_value_temp;
		max_value				= max_value_temp;
		min_value_temp			= max_value_temp = value;

		min_max_frame_index		= 0;
	}
	else
	{
		min_value_temp			= math::min( min_value_temp, value );
		max_value_temp			= math::max( max_value_temp, value );
	}

	++min_max_frame_index;

	value						= T( );
}

template < typename T >
inline void statistics_value< T >::clear( )
{
	vostok::memory::zero		( history );
	history_index				= 0;
	value						= (T)0;
}

template < typename T >
inline T statistics_value< T >::average( )
{
	return						T( );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STATISTICS_INLINE_H_INCLUDED
