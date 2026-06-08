////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_ENTRY_POINT_H_INCLUDED
#define NETWORK_CORE_ENTRY_POINT_H_INCLUDED

#include "network_core_memory.h"

namespace vostok {
namespace network_core {

void	memory_allocator				( memory::base_allocator& allocator );
std::basic_string<char,std::char_traits<char>,std::allocator<char> >
		get_ip_address					( boost::asio::io_service& io_service );
bool	get_connection_info_from_string	( pcstr buffer, char* const dest_host, u16& dest_port );
void	initialize						( );
void	finalize						( );

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_ENTRY_POINT_H_INCLUDED
