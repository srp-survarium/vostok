////////////////////////////////////////////////////////////////////////////
//	Created		: 31.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef RULE_H_INCLUDED
#define RULE_H_INCLUDED

#include <vostok/intrusive_double_linked_list.h>
#include <vostok/logging/api.h>

namespace vostok {
namespace logging {

struct initiator_filter
{
	initiator_filter *						next;
	initiator_filter *						prev;
	verbosity								verbosity;
	u32										thread_id;
#pragma warning (push)
#pragma warning (disable:4200)
	fixed_string<32>						initiator;
#pragma warning	(pop)
};

STATIC_SIZE_ASSERT(initiator_filter, 0x3C);

typedef intrusive_double_linked_list<initiator_filter, initiator_filter *, & initiator_filter::prev, & initiator_filter::next>
											filter_stack;


} // namespace logging
} // namespace vostok

#endif // #ifndef RULE_H_INCLUDED