// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_NETWORK_CORE_ENTRY_POINT_H_INCLUDED
#define VOSTOK_NETWORK_CORE_ENTRY_POINT_H_INCLUDED

namespace vostok {

namespace memory {
	class base_allocator;
} // namespace memory

namespace network_core {

void			initialize							( );
void			finalize							( );
void			memory_allocator					( memory::base_allocator& allocator );
std::string		get_ip_address						( boost::asio::io_service& io_service );
// dest_host is declared `* const` - MSVC mangles top-level pointer const from the
// declaration (target QAD; a plain `char*` mangles PAD and the symbol fails to pair).
bool			get_connection_info_from_string		( pcstr buffer, char* const dest_host, u16& dest_port );

} // namespace network_core
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_CORE_ENTRY_POINT_H_INCLUDED
