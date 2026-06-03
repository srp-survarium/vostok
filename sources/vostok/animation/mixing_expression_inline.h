////////////////////////////////////////////////////////////////////////////
//	Created		: 19.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef EXPRESSION_INLINE_H_INCLUDED
#define EXPRESSION_INLINE_H_INCLUDED

#include <vostok/animation/mixing_base_lexeme.h>

namespace vostok {
namespace animation {
namespace mixing {


// STATE[None|PARTIAL]: body byte-correct (20 bytes, == target 0x14) but objdiff can't pair -
// our /GL LTCG inlines this trivial accessor at every call site, so no standalone copy lands in
// the base mixing_expression_inline.h delink bucket (the target keeps one out-of-line copy that
// many TUs call). The inline-vs-call LTCG wall. See assembly_patterns "mixing expression operator+".
inline bool expression::is_empty				( ) const
{
	if ( m_node && m_lexeme )
		return			false;

	return				true;
}

template < typename T >
inline expression::expression					( T& lexeme )
{
	T* const cloned_lexeme = lexeme.cloned_in_buffer( );
	m_node		= cloned_lexeme;
	m_lexeme	= cloned_lexeme;
}

inline mutable_buffer& expression::buffer		( ) const
{
	ASSERT		( m_lexeme );
	return		m_lexeme->buffer( );
}

inline binary_tree_base_node& expression::node	( ) const
{
	ASSERT		( m_node );
	return		*m_node;
}

inline base_lexeme& expression::lexeme			( ) const
{
	ASSERT		( m_lexeme );
	return		*m_lexeme;
}

inline base_lexeme* expression::cloned_in_buffer( ) const
{
	ASSERT		( m_lexeme );
	return		m_lexeme;
}

template < typename T >
expression& expression::operator +=				( T& other )
{
	*this		= *this + other;
	return		*this;
}
} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef EXPRESSION_INLINE_H_INCLUDED