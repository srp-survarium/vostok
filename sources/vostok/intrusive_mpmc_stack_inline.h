// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef INTRUSIVE_MPMC_STACK_INLINE_H_INCLUDED
#define INTRUSIVE_MPMC_STACK_INLINE_H_INCLUDED

// sushi@TODO: INTRUSIVE_SPSC_QUEUE_INLINE_H_INCLUDED

namespace vostok {

template < typename T, typename BaseWithMember, T* BaseWithMember::*MemberNext >
inline	intrusive_mpmc_stack<T,BaseWithMember,MemberNext>::intrusive_mpmc_stack(  )
{
	m_top.whole = 0;
}

template < typename T, typename BaseWithMember, T* BaseWithMember::*MemberNext >
inline	intrusive_mpmc_stack<T,BaseWithMember,MemberNext>::~intrusive_mpmc_stack( )
{
}

template < typename T, typename BaseWithMember, T* BaseWithMember::*MemberNext >
inline	void	intrusive_mpmc_stack<T,BaseWithMember,MemberNext>::push( T* value )
{
	pointer_and_counter	old_head;
	pointer_and_counter new_head;
	new_head.m_pointer = value;
	do
	{
		old_head							= m_top;
		new_head.m_pointer->*MemberNext		= old_head.m_pointer;
		new_head.counter					= old_head.counter;
	}
	while ( threading::interlocked_compare_exchange	(
			m_top.whole,
			new_head.whole,
			old_head.whole ) != old_head.whole );
}

template < typename T, typename BaseWithMember, T* BaseWithMember::*MemberNext >
inline	T*	intrusive_mpmc_stack<T,BaseWithMember,MemberNext>::try_pop( )
{
	pointer_and_counter			result;
	pointer_and_counter			new_head;
	do
	{
		result					= m_top;
		if ( !result.pointer( ) )
			return NULL;

		new_head.m_pointer		= result.pointer( )->*MemberNext;
		new_head.counter		= result.counter + 1;
	}
	while ( threading::interlocked_compare_exchange	(
			m_top.whole,
			new_head.whole,
			result.whole ) != result.whole );

	return result.pointer( );
}

template < typename T, typename BaseWithMember, T* BaseWithMember::*MemberNext >
inline	bool	intrusive_mpmc_stack<T,BaseWithMember,MemberNext>::empty( ) const
{
	return					!( m_top.m_pointer );
}

template < typename T, typename BaseWithMember, T* BaseWithMember::*MemberNext >
inline	void	intrusive_mpmc_stack<T,BaseWithMember,MemberNext>::swap( T& value )
{
	VOSTOK_UNREACHABLE_CODE( "Wasn't needed just yet." );
}

} // namespace vostok

#endif // #ifndef INTRUSIVE_MPMC_STACK_INLINE_H_INCLUDED
