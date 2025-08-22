////////////////////////////////////////////////////////////////////////////
//	Created		: 11.10.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_VFS_SUB_FAT_RESOURCE_H_INCLUDED
#define VOSTOK_VFS_SUB_FAT_RESOURCE_H_INCLUDED

#include <vostok/vfs/mount_ptr.h>
#include <vostok/resources_resource_ptr.h>

namespace vostok {
namespace resources {

class vfs_sub_fat_resource : public unmanaged_resource
{
public:
	typedef resource_ptr<vfs_sub_fat_resource, resources::unmanaged_intrusive_base>		pointer;

public:
	vfs::vfs_mount_ptr					mount_ptr;
	pointer								parent; 
};

typedef vfs_sub_fat_resource::pointer		vfs_sub_fat_resource_ptr;
typedef	child_resource_ptr	<vfs_sub_fat_resource, resources::unmanaged_intrusive_base>	vfs_sub_fat_child_resource_ptr;

vfs_sub_fat_resource_ptr	get_sub_fat_resource (vfs::vfs_mount_ptr mount_ptr);

#if !VOSTOK_PLATFORM_PS3
template class VOSTOK_CORE_API	child_resource_ptr	<vfs_sub_fat_resource, resources::unmanaged_intrusive_base>;
#endif // #if !VOSTOK_PLATFORM_PS3

} // namespace resources
} // namespace vostok

#endif // #ifndef VOSTOK_VFS_SUB_FAT_RESOURCE_H_INCLUDED