////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"

namespace vostok {
namespace network {

// STATE[STUB]
void login_client_impl::on_ping_sent(
	const u32							try_count,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &try_count, &error_code, &bytes_transferred );

	// FUNCTION BODY[0x7981f0]: 13
	// <0x798207>|0x017|+0x015:'18'
	// <0x79821c>|0x02c|+0x012:'19'
	// <0x79822e>|0x03e|+0x0bd:'20'
	// <0x7982eb>|0x0fb|+0x005:'21'
	// <0>
	// <1>
	// <0x7982f0>|0x100|+0x006:'24'
	// <0x7982f6>|0x106|+0x077:'25'
	// <0x79836d>|0x17d|+0x005:'26'
	// <0>
	// <1>
	// <0x798372>|0x182|+0x039:'29'
	// <0x7983ab>|0x1bb|+0x073:'30'
	// ******
}

// STATE[STUB]
void login_client_impl::ping( u32 retry_count )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &retry_count );

	// FUNCTION BODY[0x798430]: 18
	// <0x79843f>|0x00f|+0x00f:'35'
	// <0x79844e>|0x01e|+0x005:'36'
	// <0>
	// <0x798453>|0x023|+0x006:'38'
	// <0x798459>|0x029|+0x010:'39'
	// <0x798469>|0x039|+0x005:'40'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x79846e>|0x03e|+0x0aa:'52'
	// ******
}


} // namespace network
} // namespace vostok
