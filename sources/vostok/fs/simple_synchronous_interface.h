////////////////////////////////////////////////////////////////////////////
//	Created		: 17.10.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_FS_SIMPLE_SYNC_INTERFACE_H_INCLUDED
#define VOSTOK_FS_SIMPLE_SYNC_INTERFACE_H_INCLUDED

#include <vostok/fs/windows_hdd_file_system.h>
#include <vostok/fs/asynchronous_device_interface.h>

namespace vostok {
namespace fs_new {

#if !VOSTOK_PLATFORM_PS3

class simple_synchronous_interface
{
public:
	simple_synchronous_interface			(watcher_enabled_bool);

	// order of member is important due to construction
private:
	windows_hdd_file_system					hdd_device;
	windows_hdd_file_system					dvd_device;
public:
	synchronous_device_interface			hdd_sync;
	synchronous_device_interface			dvd_sync;

	asynchronous_device_interface			hdd_async;
	asynchronous_device_interface			dvd_async;

}; // class simple_synchronous_interface

#endif // #if !VOSTOK_PLATFORM_PS3

} // namespace fs_new
} // namespace vostok

#endif // #ifndef VOSTOK_FS_SIMPLE_SYNC_INTERFACE_H_INCLUDED