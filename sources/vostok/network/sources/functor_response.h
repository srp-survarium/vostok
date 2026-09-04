////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef FUNCTOR_RESPONSE_H_INCLUDED
#define FUNCTOR_RESPONSE_H_INCLUDED

#include "response.h"

namespace vostok {
namespace network {

class functor_response : public response {
public:
	typedef boost::function< void ( ) >	functor_type;

public:
	inline	explicit	functor_response	( functor_type const& functor ) : m_functor( functor ) { }
	virtual	void		execute				( ) { m_functor( ); }

private:
	functor_type		m_functor;
}; // class functor_response

STATIC_SIZE_ASSERT(functor_response, 0x28);

} // namespace network
} // namespace vostok

#endif // #ifndef FUNCTOR_RESPONSE_H_INCLUDED
