////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef RESPONSE_H_INCLUDED
#define RESPONSE_H_INCLUDED

namespace vostok {
namespace network {

class response {
public:
	virtual			~response	( ) { }
	virtual	void	execute		( ) = 0;

	inline			response	( ) { }

	// public per the type record - the channel's intrusive_spsc_queue typedefs
	// take &response::next_for_responses without friendship
	response*		next_for_responses;
}; // class response

STATIC_SIZE_ASSERT(response, 0x8);

} // namespace network
} // namespace vostok

#endif // #ifndef RESPONSE_H_INCLUDED
