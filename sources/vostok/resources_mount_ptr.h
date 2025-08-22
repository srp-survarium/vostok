////////////////////////////////////////////////////////////////////////////
//	Created		: 09.09.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RESOURCES_MOUNT_PTR_H_INCLUDED
#define VOSTOK_RESOURCES_MOUNT_PTR_H_INCLUDED

#include <vostok/vfs/mount_ptr.h>
#include <vostok/resources_fs_task.h>

namespace vostok {
namespace resources {

class fs_task_unmount;

class VOSTOK_VFS_API intrusive_fs_task_unmount_base
{
public:
	u32			get_reference_count		() const { return m_reference_count; }

	intrusive_fs_task_unmount_base		() : m_reference_count(0) {}
	void		destroy					(fs_task_unmount * object) const;
	threading::atomic32_type			m_reference_count;
};

class mount_holder;

typedef intrusive_ptr<fs_task_unmount, intrusive_fs_task_unmount_base, threading::simple_lock >	
		mount_ptr;

template class VOSTOK_CORE_API vostok::intrusive_ptr< vostok::resources::fs_task_unmount, 
												  vostok::resources::intrusive_fs_task_unmount_base, 
												  vostok::threading::simple_lock >;

class fs_task_unmount : public fs_task, 
						public intrusive_fs_task_unmount_base
{
public:
	fs_task_unmount						(vfs_sub_fat_resource_ptr const & sub_fat_ptr);

	virtual void	execute_may_destroy_this	();

	void			link_mount_ptr		(mount_ptr ptr)
	{
		ptr->m_next						=	m_next;
		m_next							=	ptr;
	}

	vfs::vfs_mount_ptr	get_vfs_mount_ptr	() { return m_sub_fat_ptr->mount_ptr; }

private:
	void   unmount_children				(vfs::vfs_mount * sub_fat);

private:
	vfs_sub_fat_resource_ptr			m_sub_fat_ptr;

	mount_ptr							m_next;

	friend class						intrusive_fs_task_unmount_base;
	friend class						resources_manager; // temp for debug

}; // class fs_task_unmount

} // namespace resources
} // namespace vostok

#endif // #ifndef VOSTOK_RESOURCES_MOUNT_PTR_H_INCLUDED