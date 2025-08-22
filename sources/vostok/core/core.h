////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_CORE_CORE_H_INCLUDED
#define VOSTOK_CORE_CORE_H_INCLUDED

#include <vostok/logging/log_file.h>
#include <vostok/resources.h>
#include <vostok/resources_resource.h>
#include <vostok/resources_mount_ptr.h>
#include <vostok/fs/asynchronous_device_interface.h>

namespace vostok {
namespace core {

struct engine;

enum debug_initialization {
	delay_debug_initialization,
	perform_debug_initialization,
}; // enum bugtrap_usage

VOSTOK_CORE_API	void	preinitialize	(
							core::engine * engine,
							logging::log_file_usage log_file_usage,
							pcstr command_line,
							command_line::contains_application_bool,
							pcstr application,
							pcstr build_date
						);
VOSTOK_CORE_API	void	initialize	(
							pcstr lua_config_device_folder_to_save_to,
							pcstr debug_thread_id,
							debug_initialization debug_initialization
						);
VOSTOK_CORE_API	void	initialize_resources	(
							fs_new::asynchronous_device_interface &	hdd,
							fs_new::asynchronous_device_interface &	dvd,
							resources::enable_fs_watcher_bool		enable_fs_watcher
						);
VOSTOK_CORE_API	void	finalize_resources ( );
VOSTOK_CORE_API	void	finalize	( );
VOSTOK_CORE_API	void	run_tests	( );

} // namespace core
} // namespace vostok

#endif // #ifndef VOSTOK_CORE_CORE_H_INCLUDED