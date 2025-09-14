////////////////////////////////////////////////////////////////////////////
//	Created		: 11.03.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef FS_DEVICE_FILE_SYSTEM_H_INCLUDED
#define FS_DEVICE_FILE_SYSTEM_H_INCLUDED

#include <vostok/fs/platform_configuration.h>

#if VOSTOK_FS_NEW_WATCHER_ENABLED
	#include <vostok/fs/device_file_system_watcher.h>
#else
	#include <vostok/fs/device_file_system_no_watcher.h>
#endif // #if VOSTOK_FS_NEW_WATCHER_ENABLED

namespace vostok {
namespace fs_new {

enum	device_mode_enum				{ device_mode_synchronous, device_mode_asynchronous };

#if VOSTOK_FS_NEW_WATCHER_ENABLED
	typedef	device_file_system_watcher_proxy		device_file_system_proxy;
#else
	typedef	device_file_system_no_watcher_proxy		device_file_system_proxy;
#endif // #if VOSTOK_FS_NEW_WATCHER_ENABLED

} // namespace fs_new
} // namespace vostok


#endif // #ifndef FS_DEVICE_FILE_SYSTEM_H_INCLUDED