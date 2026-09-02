// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef STRING_RESPONSE_H_INCLUDED
#define STRING_RESPONSE_H_INCLUDED

#include "response.h"

namespace vostok {
namespace network {

class string_response :
	public response,
	private boost::noncopyable
{
public:
	// claude@NOTE: standalone target symbol at rva 0x49340 (NOT inlined-only); all
	// statements byte-equal - sole residual is one extra dead frame dword in the
	// target (this at -0x18 vs -0x14), LTCG slot slack with no source statement
	// mapping to it
	inline			string_response	(
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

	// claude@NOTE: the two- and three-string ctors emit NO target symbol (only the
	// one-functor ctor does); bodies by symmetry with the string_order ctor set,
	// verifiable only through callers - not anchored
	inline			string_response	(
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

	inline			string_response	(
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

	virtual			~string_response( )
	{
		pstr temp			= m_string0;
		VOSTOK_FREE_IMPL	( m_allocator, temp );

		temp				= m_string1;
		VOSTOK_FREE_IMPL	( m_allocator, temp );

		temp				= m_string2;
		VOSTOK_FREE_IMPL	( m_allocator, temp );
	}

	// claude@NOTE: the target's ICF survivor lives in string_order.h's unit (rva
	// 0x49490), so this unit's objdiff score reads None; verified by-name with
	// --view diff - zero divergent rows
	virtual	void	execute			( )
	{
		if ( m_string1 ) {
			if ( m_string2 )
				m_functor2	( m_string0, m_string1, m_string2 );
			else
				m_functor1	( m_string0, m_string1 );
		} else
			m_functor0		( m_string0 );
	}

private:
	const boost::function< void ( pcstr ) >					m_functor0;
	const boost::function< void ( pcstr, pcstr ) >			m_functor1;
	const boost::function< void ( pcstr, pcstr, pcstr ) >	m_functor2;
	char* const					m_string0;
	char* const					m_string1;
	char* const					m_string2;
	memory::base_allocator&		m_allocator;
}; // class string_response

STATIC_SIZE_ASSERT(string_response, 0x78);

} // namespace network
} // namespace vostok

#endif // #ifndef STRING_RESPONSE_H_INCLUDED
