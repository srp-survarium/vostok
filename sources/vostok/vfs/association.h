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

// claude@NOTE: target emits ctor/dtor/scalar-deleting-dtor + vtable as COMDATs in a
// vfs/association.h.obj unit (objdiff maps base_path=dummy.obj, so they show TARGET_ONLY).
// Source here already matches the target bodies (set-vptr + store type / set-vptr). Our
// base never emits these out-of-line: the only fat_node_info construction
// (saving_info_tree.cpp allocate_new_info_node) is not reached/kept in the linked base, so
// the vfs_association vtable is never referenced and the ctor/dtor are inlined away. This is
// a build/reachability + COMDAT-attribution gap, not a source edit; parked.
struct VOSTOK_VFS_API vfs_association
{
	vfs_association						(u32 type) : type(type) {}
	virtual ~vfs_association			() {}

	u32 type;
};

} // namespace vfs
} // namespace vostok

#endif // #ifndef VFS_NODES_ASSOCIATION_H_INCLUDED