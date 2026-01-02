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
}

template < typename T, typename BaseWithMember, T* BaseWithMember::*MemberNext >
inline	intrusive_mpmc_stack<T,BaseWithMember,MemberNext>::~intrusive_mpmc_stack( )
{
}

// STATE[INLINED]
template < typename T, typename BaseWithMember, T* BaseWithMember::*MemberNext >
inline	void	intrusive_mpmc_stack<T,BaseWithMember,MemberNext>::push( T* value )
{
	pointer_and_counter	old_head;
	pointer_and_counter new_head;
	do
	{
		old_head							= m_top;
		new_head.pointer( )->*MemberNext	= old_head.pointer( );
	}
	while ( threading::interlocked_compare_exchange	(
			m_top.whole,
			new_head.whole,
			old_head.whole ) != old_head.whole );
}

// STATE[UNCHECKED]
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

	// FUNCTION BODY: See multi_threading_single_size_allocator_policy< T >::allocate
	// <0>
	// <1>
	// <2>
	// <0xbe71a>|0x00a|+0x00e:'49'		result = m_top;
	// <0xbe728>|0x018|+0x006:'50'		if ( !result.pointer( ) )
	// <0xbe72e>|0x01e|+0x004:'51'			return NULL;
	// <0>
	// <0xbe732>|0x022|+0x009:'53'		new_head.counter = result.counter + 1;
	// <0xbe73b>|0x02b|+0x009:'54'		new_head.m_pointer = result.pointer( )->*MemberNext;
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0xbe744>|0x034|+0x03d:'61'
	// <0>
	// <0xbe781>|0x071|+0x003:'63'
	// ******
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
