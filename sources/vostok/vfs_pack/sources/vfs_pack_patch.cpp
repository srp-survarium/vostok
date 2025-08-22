////////////////////////////////////////////////////////////////////////////
//	Created		: 21.07.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vfs_pack.h"
#include "vfs_pack_application.h"
#include <vostok/fs/device_utils.h>
#include <vostok/fs/file_type_pointer.h>
#include <vostok/vfs/patch_args.h>

using namespace vostok;
using namespace vostok::vfs;
using namespace vostok::fs_new;

namespace vfs_pack {

bool   application::patch				()
{
	patch_args								args(m_fs_devices->hdd_sync, & m_log_format, m_log_flags);
	if ( fill_pack_arguments(args) )
		return								false;
	if ( !read_sources_string(patch_from_command, & args.from_sources) )
		return								false;
	if ( !read_sources_string(patch_to_command, & args.to_sources) )
		return								false;

	if ( !make_patch(args) )
		return								false;

	return									true;
}

} // namespace vfs_pack

