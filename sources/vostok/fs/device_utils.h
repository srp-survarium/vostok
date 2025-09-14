////////////////////////////////////////////////////////////////////////////
//	Created		: 24.02.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_FS_DEVICE_UTILS_H_INCLUDED
#define VOSTOK_FS_DEVICE_UTILS_H_INCLUDED

#include <vostok/fs/synchronous_device_interface.h>

namespace vostok {
namespace fs_new {

signalling_bool  VOSTOK_FS_API calculate_file_size	(synchronous_device_interface const &	device, 
										 file_size_type * const					out_file_size, 
										 native_path_string const &				physical_path,
										 assert_on_fail_bool					assert_on_fail = assert_on_fail_true);

bool   VOSTOK_FS_API create_folder_r		(synchronous_device_interface const &	device, 
										 native_path_string const &				path, 
										 bool									create_last);

bool   VOSTOK_FS_API create_folder_r		(synchronous_device_interface const &	device, 
										 pcstr									path, 
										 bool									create_last);

bool   VOSTOK_FS_API erase_r				(synchronous_device_interface const &	device,
										 native_path_string const &				path);

void   VOSTOK_FS_API calculate_file_hash	(synchronous_device_interface const & 	device, 
										 file_type *							file,
										 u32 * const							out_file_hash);

signalling_bool   VOSTOK_FS_API calculate_file_hash	(synchronous_device_interface const &	device, 
										 native_path_string const &				file_path,
										 u32 * const							out_file_hash);

bool   VOSTOK_FS_API	files_equal			(synchronous_device_interface const & device, 
										 file_type * f1, file_type * f2, u32 size);

// @post: position in files are changed
void   VOSTOK_FS_API	copy_data			(synchronous_device_interface const & device, 
										 file_type * f_dest, file_type * f_src, file_size_type size, 
										 u32 * hash, file_size_type size_to_hash);

} // namespace fs_new
} // namespace vostok

#endif // #ifndef VOSTOK_FS_DEVICE_UTILS_H_INCLUDED