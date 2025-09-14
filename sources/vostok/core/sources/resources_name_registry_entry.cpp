////////////////////////////////////////////////////////////////////////////
//	Created		: 28.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/resources_name_registry_entry.h>
#include <vostok/fs/path_string_utils.h>

namespace vostok {
namespace resources {

namespace detail {

u32   name_registry_hash::operator () (name_registry_entry & entry) const
{
	return	vostok::fs_new::crc32(entry.name, strings::length(entry.name));
}

bool   name_registry_equal::operator () (name_registry_entry const & left, name_registry_entry const & right) const
{
	return	strings::equal(left.name, right.name) && (left.class_id == right.class_id);
}

} // namespace detail

} // namespace resources
} // namespace vostok
