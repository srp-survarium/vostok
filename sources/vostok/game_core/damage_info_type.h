// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DAMAGE_INFO_TYPE_H_INCLUDED
#define DAMAGE_INFO_TYPE_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/fixed_string.h>
#include <vostok/fixed_vector.h>

namespace survarium {

template < int ItemSize, int ItemsCount >
struct statistics_item {
	typedef fixed_string< ItemSize > content_type;

public:
	/* 0x0000 */ fixed_string< 32 > caption;
	/* 0x002c */ fixed_vector< content_type, ItemsCount > content;
}; // struct statistics_item

struct damage_info_type : private boost::noncopyable {
public:
	/* 0x0000 */ fixed_vector< statistics_item< 46, 16 >, 20 > damage_info;
}; // struct damage_info_type

STATIC_SIZE_ASSERT(damage_info_type, 0x4F18);

} // namespace survarium

#endif // #ifndef DAMAGE_INFO_TYPE_H_INCLUDED
