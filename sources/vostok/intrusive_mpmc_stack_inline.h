////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INTRUSIVE_MPMC_STACK_INLINE_H_INCLUDED
#define INTRUSIVE_MPMC_STACK_INLINE_H_INCLUDED

namespace vostok {

// STATE[STUB]
template < typename T, typename BaseWithMember, T* BaseWithMember::*MemberNext >
inline	T*	intrusive_mpmc_stack<T,BaseWithMember,MemberNext>::try_pop( )
{
	// LOCALS
	// intrusive_mpmc_stack<survarium::bullet_manager::bullet_functor,survarium::bullet_manager::bullet_functor,72>::pointer_and_counter result
	// intrusive_mpmc_stack<survarium::bullet_manager::bullet_functor,survarium::bullet_manager::bullet_functor,72>::pointer_and_counter new_head
	// ******

	return NULL;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0xbe71a>|0x00a|+0x00e:'49'
	// <0xbe728>|0x018|+0x006:'50'
	// <0xbe72e>|0x01e|+0x004:'51'
	// <0>
	// <0xbe732>|0x022|+0x009:'53'
	// <0xbe73b>|0x02b|+0x009:'54'
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

} // namespace vostok

#endif // #ifndef INTRUSIVE_MPMC_STACK_INLINE_H_INCLUDED
