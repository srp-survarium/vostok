////////////////////////////////////////////////////////////////////////////
//	Created		: 21.07.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VFS_PACK_H_INCLUDED
#define VFS_PACK_H_INCLUDED

#include <vostok/command_line_extensions.h>
#include <vostok/fs/synchronous_device_interface.h>
#include <vostok/fs/asynchronous_device_interface.h>
#include <vostok/fs/windows_hdd_file_system.h>
//#include "../../core/sources/vfs_mount_archive.h"
#include <vostok/vfs/virtual_file_system.h>
#include <vostok/fs/portable_path_string.h>
#include <vostok/fs/native_path_string.h>
#include <vostok/fs/physical_path_iterator.h>
#include <vostok/threading_reader_writer_lock.h>
#include <vostok/fs/path_string_utils.h>
#include <vostok/fs/physical_path_info.h>
#include "vfs_pack_memory.h"

namespace vfs_pack {

extern	vostok::command_line::key				debug_info_command;
extern	vostok::command_line::key				fat_command;
extern	vostok::command_line::key				db_command;
extern	vostok::command_line::key				unpack_command;
extern	vostok::command_line::key				pack_command;
extern	vostok::command_line::key				target_platform;
extern	vostok::command_line::key				align_fat_command;
extern	vostok::command_line::key				archive_rate_command;
extern	vostok::command_line::key				fat_part_max_size_command;
extern	vostok::command_line::key				archive_part_max_size_command;
extern	vostok::command_line::key				sources_command;
extern	vostok::command_line::key				patch_from_command;
extern	vostok::command_line::key				patch_to_command;
extern	vostok::command_line::key				patch_command;
extern	vostok::command_line::key				inline_config_command;

typedef	vostok::fixed_string< 260 >		path_string;

bool   read_sources_string				(vostok::command_line::key &					key, 
										 vostok::fs_new::native_path_string * const	out_sources_string);

} // namespace vfs_pack

#endif // #ifndef VFS_PACK_H_INCLUDED