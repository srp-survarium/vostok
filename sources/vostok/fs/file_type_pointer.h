////////////////////////////////////////////////////////////////////////////
//	Created		: 20.07.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_FS_FILE_TYPE_POINTER_H_INCLUDED
#define VOSTOK_FS_FILE_TYPE_POINTER_H_INCLUDED

#include <vostok/fs/file_type.h>
#include <vostok/fs/synchronous_device_interface.h>
#include <vostok/fs/native_path_string.h>

namespace vostok {
namespace fs_new {

struct open_file_cache
{
	native_path_string			name;
	file_type *					handle;
	file_mode::mode_enum		mode;
	file_access::access_enum	access;
	u32							counter;

	open_file_cache				() : handle(NULL), counter(0) {}
};

extern open_file_cache			g_open_file_cache	[2];
extern u32						g_open_file_counter;
extern bool						g_use_open_file_cache;

inline bool   open_cached_file			(synchronous_device_interface const &	device,
										 file_type * *							out,
										 native_path_string const &				physical_path,
										 file_mode::mode_enum const				mode,
										 file_access::access_enum const			access,
										 assert_on_fail_bool const				assert_on_fail,
										 notify_watcher_bool const				notify_watcher,
										 use_buffering_bool const				use_buffering)
{
	if ( !g_use_open_file_cache )
	{
		bool const result				=	device->open(out, physical_path, mode, access, assert_on_fail, notify_watcher, use_buffering);
		if ( result )
		{
			file_type * file			=	* out;
			set_owned_by_pointer			(file, true);
		}
		return							result;
	}

	u32 min_counter						=	g_open_file_cache[0].counter;
	u32 min_index						=	0;
	for ( u32 i = 0; i < 2; ++i )
	{
		if ( g_open_file_cache[i].counter < min_counter )
		{
			min_counter				=	g_open_file_cache[i].counter;
			min_index				=	i;
		}

		if ( g_open_file_cache[i].name.length() == physical_path.length() && strings::equal(physical_path.c_str(), g_open_file_cache[i].name.c_str()) )
		{
			if ( g_open_file_cache[i].mode == mode && g_open_file_cache[i].access == access )
			{
				* out				=	g_open_file_cache[i].handle;
				return				true;
			}
		}
	}

	bool const result					=	device->open(out, physical_path, mode, access, assert_on_fail, notify_watcher, use_buffering);
	if ( result )
	{
		if ( g_open_file_cache[min_index].handle )
		{
			file_type * old_file		=	g_open_file_cache[min_index].handle;
			set_owned_by_pointer			(old_file, false);
			device->close					(old_file);
		}

		g_open_file_cache[min_index].name	=	physical_path;
		g_open_file_cache[min_index].handle	=	* out;
		g_open_file_cache[min_index].mode	=	mode;
		g_open_file_cache[min_index].access	=	access;
		g_open_file_cache[min_index].counter	=	g_open_file_counter++;

		file_type * file				=	g_open_file_cache[min_index].handle;
		set_owned_by_pointer				(file, true);
	}
	return								result;
}

class file_type_pointer : core::noncopyable
{
public:
	file_type_pointer					(native_path_string const &				physical_path,
										 synchronous_device_interface const &	device,
										 file_mode::mode_enum const				mode, 
										 file_access::access_enum const			access,
										 assert_on_fail_bool const				assert_on_fail = assert_on_fail_true,
										 notify_watcher_bool const				notify_watcher = notify_watcher_true,
										 use_buffering_bool const				use_buffering  = use_buffering_true)
		: device(device)
	{
		bool const open_result			=	open_cached_file(device, & file, physical_path, mode, access, assert_on_fail, notify_watcher, use_buffering);
		VOSTOK_UNREFERENCED_PARAMETER			(open_result);
	}

	file_type_pointer					(native_path_string const &				physical_path, 
										 synchronous_device_interface const &	device,
										 open_file_params const &				params)
		: device(device)
	{
		bool const open_result			=	device->open(& file, physical_path, params);
		VOSTOK_UNREFERENCED_PARAMETER			(open_result);
	}

	~file_type_pointer					()	{ close(); }

	operator bool						() const	{ return !!file; }
	operator file_type *				() const	{ return file; }

	void	close						()
	{ 
		if ( file && !g_use_open_file_cache )
		{
			set_owned_by_pointer			(file, false);
			device->close					(file); 
		}
		file							=	NULL;
	}

private:
	synchronous_device_interface const &	device;
	file_type *								file;

}; // class close_file_guard

} // namespace fs_new
} // namespace vostok

#endif // #ifndef VOSTOK_FS_FILE_TYPE_POINTER_H_INCLUDED
