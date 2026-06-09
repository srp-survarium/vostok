////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STRING_ORDER_H_INCLUDED
#define STRING_ORDER_H_INCLUDED

#include "order.h"

namespace vostok {
namespace network {

class string_order : public order {
public:
	// STATE[STUB]: ctors are fully inlined into their callers (no line records);
	// absent strings/functors init to NULL/empty - the matcher confirms
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
		// FUNCTION BODY[0xe7ee0]: 0
		// ******
	}

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
		// FUNCTION BODY[0x591b0]: 0
		// ******
	}

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
		// FUNCTION BODY[0xe7f80]: 0
		// ******
	}

	// STATE[PARTIAL]: three scoped free-blocks per the carcass (lines 51-58),
	// the connect_order dtor pattern; unverified vs target
	virtual			~string_order	( )
	{
		{
			pstr temp			= m_string0;
			VOSTOK_FREE_IMPL	( m_allocator, temp );
		}
		{
			pstr temp			= m_string1;
			VOSTOK_FREE_IMPL	( m_allocator, temp );
		}
		{
			pstr temp			= m_string2;
			VOSTOK_FREE_IMPL	( m_allocator, temp );
		}

		// FUNCTION BODY[0x59310]: 8
		// <0x59273>|0x013|+0x009:'51'
		// <0x5927c>|0x01c|+0x015:'52'
		// <0>
		// <0x59291>|0x031|+0x009:'54'
		// <0x5929a>|0x03a|+0x015:'55'
		// <0>
		// <0x592af>|0x04f|+0x009:'57'
		// <0x592b8>|0x058|+0x015:'58'
		// ******
	}

	// STATE[STUB]
	virtual	void	execute			( )
	{
		// FUNCTION BODY[0x59490]: 7
		// <0x5949f>|0x00f|+0x00c:'62'
		// <0x594ab>|0x01b|+0x00c:'63'
		// <0x594b7>|0x027|+0x02c:'64'
		// <0x594e3>|0x053|+0x002:'65'
		// <0x594e5>|0x055|+0x022:'66'
		// <0x59507>|0x077|+0x002:'67'
		// <0x59509>|0x079|+0x018:'68'
		// ******
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
