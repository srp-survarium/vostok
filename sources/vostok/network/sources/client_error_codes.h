////////////////////////////////////////////////////////////////////////////
//	Created		: 22.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_NETWORK_CLIENT_ERROR_CODES_H_INCLUDED
#define VOSTOK_NETWORK_CLIENT_ERROR_CODES_H_INCLUDED

namespace vostok {
namespace network {

enum client_error_codes_enum {
	host_cannot_be_resolved,
	server_cannot_be_connected,
	unable_to_write_to_socket,
	unable_to_read_from_socket,
}; // enum client_error_codes_enum

} // namespace network
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_CLIENT_ERROR_CODES_H_INCLUDED