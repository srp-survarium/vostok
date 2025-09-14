////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VFS_NODES_ASSOCIATION_H_INCLUDED
#define VFS_NODES_ASSOCIATION_H_INCLUDED

#include <vostok/vfs/api.h>

namespace vostok {
namespace vfs {

struct fat_node_info;

struct VOSTOK_VFS_API vfs_association
{
	vfs_association						(u32 type) : type(type) {}
	virtual ~vfs_association			() {}

	u32 type;
};

} // namespace vfs
} // namespace vostok

#endif // #ifndef VFS_NODES_ASSOCIATION_H_INCLUDED