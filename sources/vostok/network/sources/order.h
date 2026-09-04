////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED

namespace vostok {
namespace network {

class order : private boost::noncopyable {
public:
	virtual			~order	( ) { }
	virtual	void	execute	( ) = 0;

	// public per the type record - the channel's intrusive_spsc_queue typedefs
	// take &order::next_for_orders without friendship
	order*			next_for_orders;
}; // class order

STATIC_SIZE_ASSERT(order, 0x8);

} // namespace network
} // namespace vostok

#endif // #ifndef ORDER_H_INCLUDED
