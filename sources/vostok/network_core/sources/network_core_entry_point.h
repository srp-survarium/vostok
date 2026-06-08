////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_ENTRY_POINT_H_INCLUDED
#define NETWORK_CORE_ENTRY_POINT_H_INCLUDED

namespace vostok {

namespace memory { class base_allocator; }

namespace network_core {

void			memory_allocator				( memory::base_allocator& allocator );
std::string		get_ip_address					( boost::asio::io_service& io_service );
bool			get_connection_info_from_string	( pcstr buffer, char* dest_host, u16& dest_port );
void			initialize						( );
void			finalize						( );

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_ENTRY_POINT_H_INCLUDED
