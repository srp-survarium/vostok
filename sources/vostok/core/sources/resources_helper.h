////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef RESOURCES_HELPER_H_INCLUDED
#define RESOURCES_HELPER_H_INCLUDED

namespace vostok {
namespace resources {
	class			fs_iterator;
} // namespace resources
} // namespace vostok

#include "resources_macros.h"
#include <vostok/resources.h>
#include <vostok/resources_fs.h>
#include <vostok/fs_path.h>

namespace vostok {
namespace resources {

#define USER_ALLOCATOR	::vostok::memory::g_resources_helper_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

//----------------------------------------------------------
// general
//----------------------------------------------------------
class				queries_result;

typedef	fastdelegate::FastDelegate< void (bool result) >	store_callback;

class				query_result;

class raw_file_buffer_helper
{
public:
	raw_file_buffer_helper *	next;
	class_id_enum				resource_class;
};

} // namespace resources
} // namespace vostok

#endif // #ifndef RESOURCES_HELPER_H_INCLUDED