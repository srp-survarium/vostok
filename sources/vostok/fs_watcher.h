////////////////////////////////////////////////////////////////////////////
//	Created		: 30.09.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_FS_WATCHER_H_INCLUDED
#define VOSTOK_FS_WATCHER_H_INCLUDED

#include <vostok/fs_path.h>

namespace vostok {
namespace fs {

struct VOSTOK_CORE_API file_change_info
{
	enum				change_type_enum	{	change_type_unset,
												change_type_added, 
												change_type_removed,
												change_type_modified,
												change_type_renamed,	};
	change_type_enum	type;
	path_string			physical_file_path;
	path_string			physical_renamed_old_file_path;
	path_string			physical_renamed_new_file_path;

	path_string			logical_file_path;
	path_string			logical_renamed_old_file_path;
	path_string			logical_renamed_new_file_path;
	bool				do_resource_deassociation;

	file_change_info	() : type(change_type_unset), do_resource_deassociation(false) {}
};

typedef fastdelegate::FastDelegate< void (file_change_info const & info) >	file_change_callback;

#ifndef MASTER_GOLD

void  VOSTOK_CORE_API 	subscribe_watcher					(pcstr logical_path, file_change_callback);
void  VOSTOK_CORE_API 	unsubscribe_watcher					(pcstr logical_path, file_change_callback);
void					notify_subscribers					(file_change_info & info);

#endif // #ifndef MASTER_GOLD

} // namespace fs
} // namespace vostok

#endif // #ifndef VOSTOK_FS_WATCHER_H_INCLUDED