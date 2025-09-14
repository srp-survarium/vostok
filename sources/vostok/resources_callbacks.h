////////////////////////////////////////////////////////////////////////////
//	Created		: 27.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RESOURCES_CALLBACKS_H_INCLUDED
#define VOSTOK_RESOURCES_CALLBACKS_H_INCLUDED

#include <vostok/resources_memory_usage.h>

namespace vostok {
namespace resources {

class					resource_base;
typedef					boost::function< void ( resource_base * resource ) >	query_finished_callback;
VOSTOK_CORE_API	void	set_query_finished_callback	( query_finished_callback callback );

class					query_result;

typedef					boost::function< void ( query_result * out_of_memory_query ) >
						out_of_memory_callback;

class					memory_type;
typedef					boost::function< void ( query_result * const destruction_observer, memory_usage_type const & memory_usage, class_id_enum class_id ) >
						resource_freed_callback;

VOSTOK_CORE_API	void	set_out_of_memory_callback	(out_of_memory_callback callback);
VOSTOK_CORE_API	void	set_resource_freed_callback	(resource_freed_callback callback);
out_of_memory_callback	get_out_of_memory_callback	();
resource_freed_callback	get_resource_freed_callback	();

} // namespace resources
} // namespace vostok

#endif // #ifndef VOSTOK_RESOURCES_CALLBACKS_H_INCLUDED