#include "pch.h"
#include "anchor.h"

#include <vostok/network_core/udp_match_packet.h>

namespace vostok
{
	// Shared opaque sink used by the anchor TUs.
	void example_callback(const char *name)
	{
		printf("%s\n", name);
	}

	void anchor_network_core( )
	{
		// Real callers inline this helper, while the target keeps a standalone COMDAT.
		u8 ( network_core::udp_match_packet::*header_size_ptr )( ) const = &network_core::udp_match_packet::header_size;
		example_callback( ( pcstr )&header_size_ptr );
	}
}
