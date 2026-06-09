////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef FUNCTOR_ORDER_H_INCLUDED
#define FUNCTOR_ORDER_H_INCLUDED

#include "order.h"

namespace vostok {
namespace network {

class functor_order : public order {
public:
	typedef boost::function< void ( ) >	functor_type;

public:
	inline			functor_order	( functor_type const& functor ) : m_functor( functor ) { }
	virtual	void	execute			( ) { m_functor( ); }

private:
	functor_type	m_functor;
}; // class functor_order

} // namespace network
} // namespace vostok

#endif // #ifndef FUNCTOR_ORDER_H_INCLUDED