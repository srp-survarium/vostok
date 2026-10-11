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
	typedef boost::function< void ( pcstr ) >			functor_type0;
	typedef boost::function< void ( pcstr, pcstr ) >		functor_type1;
	typedef boost::function< void ( pcstr, pcstr, pcstr ) >	functor_type2;





	inline			string_response	(
			memory::base_allocator&		allocator,
			functor_type0 const&	functor,
			pcstr const					string0
		) :
		m_functor0	( functor ),
		m_functor1	( ),
		m_functor2	( ),
		m_string0	( strings::duplicate( allocator, string0 ) ),
		m_string1	( NULL ),
		m_string2	( NULL ),
		m_allocator	( allocator )
#line 31
	{
	}



	// no shipped symbol: mirrors the string_order ctor set
	inline			string_response	(
			memory::base_allocator&		allocator,
			functor_type1 const&	functor,
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
			functor_type2 const&	functor,
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
#line 52
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
		if ( m_string1 ) {
			if ( m_string2 )
				m_functor2	( m_string0, m_string1, m_string2 );
			else
				m_functor1	( m_string0, m_string1 );
		} else
			m_functor0		( m_string0 );
	}

private:
	const functor_type0		m_functor0;
	const functor_type1		m_functor1;
	const functor_type2		m_functor2;
	char* const					m_string0;
	char* const					m_string1;
	char* const					m_string2;
	memory::base_allocator&		m_allocator;
}; // class string_response

STATIC_SIZE_ASSERT(string_response, 0x78);

} // namespace network
} // namespace vostok

#endif // #ifndef STRING_RESPONSE_H_INCLUDED
