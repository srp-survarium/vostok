////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STRING_RESPONSE_H_INCLUDED
#define STRING_RESPONSE_H_INCLUDED

#include "response.h"

namespace vostok {
namespace network {

class string_response :
	public response,
	public boost::noncopyable
{
public:
	// STATE[STUB]: ctors are fully inlined into their callers (no line records);
	// absent strings/functors init to NULL/empty - the matcher confirms
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
		// FUNCTION BODY[0x59340]: 0
		// ******
	}

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

	// STATE[PARTIAL]: three scoped free-blocks per the carcass (lines 53-60);
	// unverified vs target
	virtual			~string_response( )
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

		// FUNCTION BODY[0x593e0]: 8
		// <0x593f3>|0x013|+0x009:'53'
		// <0x593fc>|0x01c|+0x015:'54'
		// <0>
		// <0x59411>|0x031|+0x009:'56'
		// <0x5941a>|0x03a|+0x015:'57'
		// <0>
		// <0x5942f>|0x04f|+0x009:'59'
		// <0x59438>|0x058|+0x015:'60'
		// ******
	}

	// STATE[STUB]: no code attributed to this header in the target (the body
	// ICF-folds with string_order::execute)
	virtual	void	execute			( )
	{
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
