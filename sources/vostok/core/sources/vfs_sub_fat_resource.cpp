////////////////////////////////////////////////////////////////////////////
//	Created		: 11.10.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/vfs_sub_fat_resource.h>
#include <vostok/resources_resource_ptr.h>

namespace vostok {
namespace resources {

#if VOSTOK_PLATFORM_PS3
template class VOSTOK_CORE_API child_resource_ptr	<vfs_sub_fat_resource, resources::unmanaged_intrusive_base>;
#endif // #if VOSTOK_PLATFORM_PS3

vfs_sub_fat_resource_ptr	get_sub_fat_resource (vfs::vfs_mount_ptr mount_ptr)
{
	return									(vfs_sub_fat_resource *)mount_ptr->user_data;
}

} // namespace resources
} // namespace vostok
