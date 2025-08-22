////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.02.2009
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RESOURCES_FS_ITERATOR_H_INCLUDED
#define VOSTOK_RESOURCES_FS_ITERATOR_H_INCLUDED

#include <vostok/fs_path_string.h>
#include <vostok/resources_resource.h>
#include <vostok/fs_platform_configuration.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/fs/native_path_string.h>
#include <vostok/vfs/mount_args.h>
#include <vostok/resources_mount_ptr.h>
#include <vostok/vfs/locked_iterator.h>
#include <vostok/vfs/watcher_notification.h>
#include <vostok/fs/synchronous_device_interface.h>

namespace vostok {
namespace resources {

class	fs_task_composite;
enum	recursive_bool					{ recursive_false, recursive_true };

typedef	boost::function< void (mount_ptr result) >								query_mount_callback;
typedef	boost::function< void (bool result) >									query_erase_callback;
typedef	boost::function< void (vfs::vfs_locked_iterator const & iterator) >		query_vfs_iterator_callback;

void VOSTOK_CORE_API	query_mount					(pcstr 						mount_id,
												 query_mount_callback		callback,
												 memory::base_allocator*	allocator);

void VOSTOK_CORE_API	query_mount_physical		(fs_new::virtual_path_string const &	virtual_path, 
												 fs_new::native_path_string const &		physical_path, 
												 pcstr									descriptor,
												 fs_new::watcher_enabled_bool const		watcher_enabled,
						 						 query_mount_callback const				callback, 
						 						 memory::base_allocator * const			allocator,
						 						 recursive_bool const					recursive,
						 						 fs_task_composite * const				composite_task = NULL);

void VOSTOK_CORE_API	query_mount_archive			(fs_new::virtual_path_string const &	virtual_path,
										 		 fs_new::native_path_string const & 	fat_physical_path,
										 		 fs_new::native_path_string const & 	archive_physical_path,
												 pcstr									descriptor,
										 		 query_mount_callback const &			callback,
										 		 memory::base_allocator * const			allocator,
										 		 fs_task_composite * const				composite_task = NULL);

void VOSTOK_CORE_API	query_erase_file			(fs_new::native_path_string const &		opt_physical_path, 
												 fs_new::virtual_path_string const &	opt_virtual_path, 
												 query_erase_callback const &			callback, 
												 memory::base_allocator * 				allocator);

void VOSTOK_CORE_API	query_vfs_iterator			(fs_new::virtual_path_string const &	path, 
												 query_vfs_iterator_callback const &	callback,
												 memory::base_allocator *				allocator,
												 recursive_bool							recursive = recursive_false,
												 query_result_for_cook *				parent = NULL);

void VOSTOK_CORE_API	query_vfs_iterator_and_wait	(fs_new::virtual_path_string const &	path, 
												 query_vfs_iterator_callback const &	callback,
												 memory::base_allocator *				allocator,
												 recursive_bool							recursive = recursive_false,
												 query_result_for_cook *				parent = NULL);

bool VOSTOK_CORE_API	convert_physical_to_virtual_path	(fs_new::virtual_path_string *			out_path, 
														 fs_new::native_path_string	const &		path,
														 pcstr									mount_descriptor);

bool VOSTOK_CORE_API	convert_virtual_to_physical_path	(fs_new::native_path_string *			out_path, 
														 fs_new::virtual_path_string const &	path,
														 pcstr									mount_descriptor);

fs_new::physical_path_info   VOSTOK_CORE_API get_physical_path_info	(vfs::vfs_iterator const & it);

fs_new::physical_path_info   VOSTOK_CORE_API get_physical_path_info	(fs_new::native_path_string const & path, 
																	 bool check_archive_on_the_way = false);

fs_new::physical_path_info   VOSTOK_CORE_API get_physical_path_info	(fs_new::virtual_path_string const & path, 
																	 pcstr mount_descriptor, 
																	 bool check_archive_on_the_way = true);

#if VOSTOK_FS_NEW_WATCHER_ENABLED

u32		VOSTOK_CORE_API	subscribe_watcher	(fs_new::virtual_path_string const & virtual_path, vfs::vfs_notification_callback const &);
void	VOSTOK_CORE_API	unsubscribe_watcher	(u32 subscriber_id);

#endif // #if VOSTOK_FS_NEW_WATCHER_ENABLED

VOSTOK_CORE_API fs_new::synchronous_device_interface &	get_synchronous_device	();

} // namespace resources
} // namespace vostok

#endif // #ifndef VOSTOK_RESOURCES_FAT_ITERATOR_H_INCLUDED