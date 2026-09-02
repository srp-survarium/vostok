// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INTRUSIVE_MPMC_STACK_H_INCLUDED
#define INTRUSIVE_MPMC_STACK_H_INCLUDED

namespace vostok {

template < typename T, typename BaseWithMember, T* BaseWithMember::*MemberNext >
class VOSTOK_CORE_API intrusive_mpmc_stack {
public:
	typedef T			value_type;
	typedef value_type*	pointer_type;

public:
	inline			intrusive_mpmc_stack	( );
	inline			~intrusive_mpmc_stack	( );

	inline	void	push					( T* value );
	inline	T*		try_pop					( );
	inline	bool	empty					( ) const;

	inline	void	swap					( T& value );

private:

#	pragma warning(push)
#	pragma warning(disable:4201)
	union pointer_and_counter
	{
		threading::atomic64_type	whole;
		struct {
			T*			m_pointer;
			u32			counter;
		};

		inline T* pointer	( )
		{
			return m_pointer;
		}
	};
#	pragma warning(pop)

	pointer_and_counter	m_top;
}; // class intrusive_mpmc_stack

} // namespace vostok

#include <vostok/intrusive_mpmc_stack_inline.h>

#endif // #ifndef INTRUSIVE_MPMC_STACK_H_INCLUDED
