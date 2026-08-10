////////////////////////////////////////////////////////////////////////////
//	Created		: 19.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef ADDITION_LEXEME_H_INCLUDED
#define ADDITION_LEXEME_H_INCLUDED

#include <vostok/animation/mixing_binary_tree_addition_node.h>
#include <vostok/animation/mixing_binary_operation_lexeme.h>
#include <vostok/animation/mixing_expression.h>

namespace vostok {
namespace animation {
namespace mixing {

class VOSTOK_ANIMATION_API addition_lexeme :
	public binary_tree_addition_node,
	public binary_operation_lexeme
{
public:
	template < typename T1, typename T2 >
	inline					addition_lexeme		( T1& left, T2& right );
	inline explicit			addition_lexeme		( addition_lexeme const& other );
	inline	addition_lexeme* cloned_in_buffer	( );
	virtual					~addition_lexeme	( );

protected:
	using binary_operation_lexeme::cloned_in_buffer;
}; // class addition_lexeme

STATIC_SIZE_ASSERT(addition_lexeme, 0x24);

	template < typename T1, typename T2 >
	inline addition_lexeme&	operator +			( T1& left, T2& right );
	template < typename T >
	inline expression		operator +			( expression& left, T& right );
	template < typename T >
	inline expression		operator +			( T& left, expression& right );
	inline expression		operator +			( expression& left, expression& right );
	inline expression		operator +			( expression const& left, expression& right );
	inline expression		operator +			( expression& left, expression const& right );
	inline expression		operator +			( expression const& left, expression const& right );

} // namespace mixing
} // namespace animation
} // namespace vostok

#include <vostok/animation/mixing_addition_lexeme_inline.h>

#endif // #ifndef ADDITION_LEXEME_H_INCLUDED
