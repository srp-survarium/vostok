////////////////////////////////////////////////////////////////////////////
//	Created		: 03.02.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SYNCHRONOUS_DEVICE_INTERFACE_H_INCLUDED
#define SYNCHRONOUS_DEVICE_INTERFACE_H_INCLUDED

#include <vostok/fs/device_file_system_proxy.h>
#include <vostok/fs/asynchronous_device_commands.h>

namespace vostok {
namespace vfs
{
class	query_mount_arguments;
class	archive_mounter;
struct	mount_root_node_functions;
} // namespace vfs
namespace fs_new {

class asynchronous_device_interface;

class VOSTOK_FS_API synchronous_device_interface : private core::noncopyable
{
public:
	synchronous_device_interface		(device_file_system_interface * device, watcher_enabled_bool watcher_enabled);
	synchronous_device_interface		(asynchronous_device_interface * adi, memory::base_allocator * allocator);
	synchronous_device_interface		(device_file_system_proxy device) : m_device(device), m_synchronize_query(NULL), m_out_of_memory(false) {}

	synchronous_device_interface		(asynchronous_device_interface * adi, memory::base_allocator * allocator, 
										 device_file_system_interface * device, watcher_enabled_bool watcher_enabled);

	device_file_system_proxy const &	operator ->	() const { return m_device; }
	bool	out_of_memory				() { return m_out_of_memory; }

	~synchronous_device_interface		();

private:
	void								initialize_private	(device_file_system_proxy const & device, synchronize_device_query * query);
	void								get_synchronous_access	(asynchronous_device_interface * adi, memory::base_allocator * allocator);
	void								set_out_of_memory	() { m_out_of_memory = true; }
	device_file_system_proxy const &	get_device			() { return m_device; }

private:
	synchronous_device_interface		(synchronous_device_interface &) {}
	friend class						asynchronous_device_interface;

	synchronize_device_query *			m_synchronize_query;
	device_file_system_proxy			m_device;
	bool								m_out_of_memory;

	friend class						::vostok::vfs::query_mount_arguments;
	friend class						::vostok::vfs::archive_mounter;
	friend struct						::vostok::vfs::mount_root_node_functions;

}; // synchronous_interface

} // namespace fs_new
} // namespace vostok

#endif // #ifndef SYNCHRONOUS_DEVICE_INTERFACE_H_INCLUDED