// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef NETWORK_CORE_DISCONNECT_EVENT_TYPES_ENUM_H_INCLUDED
#define NETWORK_CORE_DISCONNECT_EVENT_TYPES_ENUM_H_INCLUDED

namespace vostok {
namespace network_core {

enum disconnect_event_types_enum
{
	disconnected_by_timeout				= 0x0,
	disconnected_by_connection_lost		= 0x1,
	disconnected_by_initiator			= 0x2,
};

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_DISCONNECT_EVENT_TYPES_ENUM_H_INCLUDED
