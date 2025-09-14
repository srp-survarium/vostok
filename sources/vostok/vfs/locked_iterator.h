////////////////////////////////////////////////////////////////////////////
//	Created		: 04.05.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_VFS_SUB_FAT_PIN_VFS_ITERATOR_H_INCLUDED
#define VOSTOK_VFS_SUB_FAT_PIN_VFS_ITERATOR_H_INCLUDED

#include <vostok/vfs/iterator.h>

namespace vostok {
namespace vfs {

class VOSTOK_VFS_API vfs_locked_iterator : public vfs_iterator
{
public:
			vfs_locked_iterator			() : mount_operation_id(0) {}
			~vfs_locked_iterator		();	

	void	clear						();

	static	vfs_locked_iterator	end		() { return	vfs_locked_iterator(); }

	void	grab						(vfs_locked_iterator const & other);
	void	assign						(base_node<> * node, vfs_hashset * hashset, type_enum type, u32 mount_operation_id);

public: // really private
	void	clear_without_unpin			();
private:
	void	operator =					(vfs_iterator)					{ NOT_IMPLEMENTED(); }
			vfs_locked_iterator			(vfs_locked_iterator const &)	{ NOT_IMPLEMENTED(); }
	void	unlock_and_decref_if_needed	();

private:
	u32		mount_operation_id;

}; // class vfs_locked_iterator

} // namespace vfs
} // namespace vostok

#endif // #ifndef VOSTOK_VFS_SUB_FAT_PIN_VFS_ITERATOR_H_INCLUDED