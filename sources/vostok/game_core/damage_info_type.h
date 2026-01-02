////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_INFO_TYPE_H_INCLUDED
#define DAMAGE_INFO_TYPE_H_INCLUDED

#include <vostok/ai/npc_statistics.h>

namespace survarium {

struct damage_info_type : public boost::noncopyable {
	fixed_vector< ai::statistics_item< 46, 16 >, 20 >	damage_info;
}; // struct damage_info_type

STATIC_SIZE_ASSERT(damage_info_type, 0x4F18);

} // namespace survarium

#endif // #ifndef DAMAGE_INFO_TYPE_H_INCLUDED
