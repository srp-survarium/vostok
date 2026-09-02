// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_CLIENT_ERROR_CODES_ENUM_H_INCLUDED
#define NETWORK_CORE_CLIENT_ERROR_CODES_ENUM_H_INCLUDED

namespace vostok {
namespace network_core {

enum client_error_codes_enum
{
	host_cannot_be_resolved			= 0x0,
	unable_to_write_to_socket		= 0x1,
	unable_to_read_from_socket		= 0x2,
	server_cannot_be_connected		= 0x3,
};

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_CLIENT_ERROR_CODES_ENUM_H_INCLUDED
