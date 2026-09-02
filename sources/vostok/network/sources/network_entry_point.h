// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_NETWORK_ENTRY_POINT_H_INCLUDED
#define VOSTOK_NETWORK_ENTRY_POINT_H_INCLUDED

namespace vostok {
namespace network {

// create_world/destroy_world/memory_allocator are declared in <vostok/network/api.h>
void	initialize	( );
void	finalize	( );

} // namespace network
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_ENTRY_POINT_H_INCLUDED
