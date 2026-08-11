////////////////////////////////////////////////////////////////////////////
//	Created		: 19.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef ADDITION_LEXEME_INLINE_H_INCLUDED
#define ADDITION_LEXEME_INLINE_H_INCLUDED

namespace vostok {
namespace animation {
namespace mixing {

template < typename T1, typename T2 >
inline addition_lexeme::addition_lexeme						( T1& left, T2& right ) :
	binary_tree_addition_node( cloned_in_buffer(left), cloned_in_buffer(right) ),
	binary_operation_lexeme ( left.buffer() )
{
	ASSERT				( left.buffer() == right.buffer() );
}

inline addition_lexeme::addition_lexeme						( addition_lexeme const& other ) :
	binary_tree_addition_node( other ),
	binary_operation_lexeme ( other )
{
}

inline addition_lexeme* addition_lexeme::cloned_in_buffer	( )
{
	return				base_lexeme::cloned_in_buffer< addition_lexeme >( );
}

template < typename T1, typename T2 >
inline addition_lexeme& operator +							( T1& left, T2& right )
{
	return				*addition_lexeme( left, right ).cloned_in_buffer( );
}

template < typename T >
inline expression operator +								( expression& left, T& right )
{
	if ( left.is_empty( ) )
		return			expression( right );

	return				expression( *addition_lexeme( left, right ).cloned_in_buffer( ) );
}

template < typename T >
inline expression operator +								( T& left, expression& right )
{
	if ( right.is_empty( ) )
		return			expression( left );

	return				expression( *addition_lexeme( left, right ).cloned_in_buffer( ) );
}

inline expression operator +								( expression& left, expression& right )
{
	if ( left.is_empty( ) )
		return			expression( right );

	if ( right.is_empty( ) )
		return			expression( left );

	return				expression( *addition_lexeme( left, right ).cloned_in_buffer( ) );
}

inline expression operator +								( expression const& left, expression& right )
{
	if ( left.is_empty( ) )
		return			expression( right );

	if ( right.is_empty( ) )
		return			expression( left );

	expression left_real	( left );
	return				expression( *addition_lexeme( left_real, right ).cloned_in_buffer( ) );
}

inline expression operator +								( expression& left, expression const& right )
{
	if ( left.is_empty( ) )
		return			expression( right );

	if ( right.is_empty( ) )
		return			expression( left );

	expression right_real	( right );
	return				expression( *addition_lexeme( left, right_real ).cloned_in_buffer( ) );
}

inline expression operator +								( expression const& left, expression const& right )
{
	if ( left.is_empty( ) )
		return			expression( right );

	if ( right.is_empty( ) )
		return			expression( left );

	expression left_real	( left );
	expression right_real	( right );
	return				expression( *addition_lexeme( left_real, right_real ).cloned_in_buffer( ) );
}

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef ADDITION_LEXEME_INLINE_H_INCLUDED
