// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef STRING_ORDER_H_INCLUDED
#define STRING_ORDER_H_INCLUDED

#include "order.h"

namespace vostok {
namespace network {

class string_order : public order {
public:
	// claude@NOTE: only diff is `sub esp` - target reserves one untouched ghost
	// temp dword per strings::duplicate call; zero PDB locals on either side,
	// whole-program LTCG inline-consideration context (assembly_patterns.md)
	inline			string_order	(
			memory::base_allocator&		allocator,
			boost::function< void ( pcstr ) > const&	functor,
			pcstr const					string0
		) :
		m_functor0	( functor ),
		m_functor1	( ),
		m_functor2	( ),
		m_string0	( strings::duplicate( allocator, string0 ) ),
		m_string1	( NULL ),
		m_string2	( NULL ),
		m_allocator	( allocator )
	{
	}

	// claude@NOTE: same ghost-frame-dword residual as the 1-string ctor (x2)
	inline			string_order	(
			memory::base_allocator&		allocator,
			boost::function< void ( pcstr, pcstr ) > const&	functor,
			pcstr const					string0,
			pcstr const					string1
		) :
		m_functor0	( ),
		m_functor1	( functor ),
		m_functor2	( ),
		m_string0	( strings::duplicate( allocator, string0 ) ),
		m_string1	( strings::duplicate( allocator, string1 ) ),
		m_string2	( NULL ),
		m_allocator	( allocator )
	{
	}

	// claude@NOTE: same ghost-frame-dword residual as the 1-string ctor (x3)
	inline			string_order	(
			memory::base_allocator&		allocator,
			boost::function< void ( pcstr, pcstr, pcstr ) > const&	functor,
			pcstr const					string0,
			pcstr const					string1,
			pcstr const					string2
		) :
		m_functor0	( ),
		m_functor1	( ),
		m_functor2	( functor ),
		m_string0	( strings::duplicate( allocator, string0 ) ),
		m_string1	( strings::duplicate( allocator, string1 ) ),
		m_string2	( strings::duplicate( allocator, string2 ) ),
		m_allocator	( allocator )
	{
	}

	virtual			~string_order	( )
	{
		pstr temp			= m_string0;
		VOSTOK_FREE_IMPL	( m_allocator, temp );

		temp				= m_string1;
		VOSTOK_FREE_IMPL	( m_allocator, temp );

		temp				= m_string2;
		VOSTOK_FREE_IMPL	( m_allocator, temp );
	}

	virtual	void	execute			( )
	{
		if ( m_string1 )
			if ( m_string2 )
				m_functor2	( m_string0, m_string1, m_string2 );
			else
				m_functor1	( m_string0, m_string1 );
		else
			m_functor0	( m_string0 );
	}

private:
	const boost::function< void ( pcstr ) >					m_functor0;
	const boost::function< void ( pcstr, pcstr ) >			m_functor1;
	const boost::function< void ( pcstr, pcstr, pcstr ) >	m_functor2;
	char* const					m_string0;
	char* const					m_string1;
	char* const					m_string2;
	memory::base_allocator&		m_allocator;
}; // class string_order

STATIC_SIZE_ASSERT(string_order, 0x78);

} // namespace network
} // namespace vostok

#endif // #ifndef STRING_ORDER_H_INCLUDED
