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

inline addition_lexeme::addition_lexeme						( addition_lexeme const& other, bool ) :
	binary_tree_addition_node( other ),
	binary_operation_lexeme ( other, true )
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

// STATE[12.09%|PARTIAL]: target compiles this template instantiation with FULL
// optimization (FPO, register allocation, is_empty + addition_lexeme ctor/cloned_in_buffer
// + intrusive_ptr ref-counting all inlined); base is /Od. Per-TU build-config divergence,
// not source-steerable. See .md / assembly_patterns "mixing expression operator+".
template < typename T >
inline expression operator +								( expression& left, T& right )
{
	if ( left.is_empty( ) )
		return			right;

	return				expression( *addition_lexeme( left, right ).cloned_in_buffer( ) );
}

// STATE[None|NOT-EMITTED]: no base ODR-use yet (caller weapon_and_hands_expression still STUB).
inline expression operator +								( expression& left, expression& right )
{
	if ( left.is_empty( ) )
		return			right;

	if ( right.is_empty( ) )
		return			left;

	return				expression( *addition_lexeme( left, right ).cloned_in_buffer( ) );
}

// STATE[None|NOT-EMITTED]: no base ODR-use yet (caller weapon_and_hands_expression still STUB).
inline expression operator +								( expression& left, expression const& right )
{
	if ( left.is_empty( ) )
		return			right;

	if ( right.is_empty( ) )
		return			left;

	expression			non_const_right = right;
	return				expression( *addition_lexeme( left, non_const_right ).cloned_in_buffer( ) );
}

// STATE[45.98%|PARTIAL]: structure matches target exactly; residual is the whole-program
// inline-vs-call LTCG class x3 - target keeps expression::is_empty, addition_lexeme::cloned_in_buffer,
// and ~addition_lexeme as out-of-line calls; our /GL inlines all three (byte temps for is_empty,
// add ecx,1Ch -> base_lexeme::cloned_in_buffer, explicit member ~expression dtors). Not
// source-steerable. See .md / assembly_patterns "mixing expression operator+".
inline expression operator +								( expression const& left, expression const& right )
{
	if ( left.is_empty( ) )
		return			right;

	if ( right.is_empty( ) )
		return			left;

	expression			non_const_left = left;
	expression			non_const_right = right;
	return				expression( *addition_lexeme( non_const_left, non_const_right ).cloned_in_buffer( ) );
}

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef ADDITION_LEXEME_INLINE_H_INCLUDED