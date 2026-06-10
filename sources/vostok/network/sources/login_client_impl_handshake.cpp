////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"

namespace vostok {
namespace network {

// STATE[STUB]
void login_client_impl::on_handshaked(
	boost::system::error_code const&	error_code,
	boost::function< void( enum handshaking_error_types_enum ) > const&	functor,
	u32									retry_count,
	const bool							stop_timer
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &error_code, &functor, &retry_count, &stop_timer );

	// FUNCTION BODY[0x798520]: 18
	// <0x79853a>|0x01a|+0x00c:'37'
	// <0x798546>|0x026|+0x015:'38'
	// <0x79855b>|0x03b|+0x010:'39'
	// <0>
	// <0x79856b>|0x04b|+0x00a:'41'
	// <0x798575>|0x055|+0x080:'42'
	// <0x7985f5>|0x0d5|+0x0de:'43'
	// <0x7986d3>|0x1b3|+0x01b:'44'
	// <0>
	// <0x7986ee>|0x1ce|+0x002:'46'
	// <0x7986f0>|0x1d0|+0x00a:'47'
	// <0>
	// <0x7986fa>|0x1da|+0x005:'49'
	// <0>
	// <1>
	// <0x7986ff>|0x1df|+0x080:'52'
	// <0x79877f>|0x25f|+0x010:'53'
	// <0x79878f>|0x26f|+0x00a:'54'
	// ******
}

// STATE[STUB]
void login_client_impl::handshake(
	boost::function< void( enum handshaking_error_types_enum ) > const&	functor,
	const u32		retry_count,
	const bool		stop_timer
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &functor, &retry_count, &stop_timer );

	// FUNCTION BODY[0x7987a0]: 26
	// <0x7987b7>|0x017|+0x00f:'59'
	// <0x7987c6>|0x026|+0x00a:'60'
	// <0x7987d0>|0x030|+0x005:'61'
	// <0>
	// <1>
	// <0x7987d5>|0x035|+0x077:'64'
	// <0>
	// <0x79884c>|0x0ac|+0x00c:'66'
	// <0x798858>|0x0b8|+0x010:'67'
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
	// <0x798868>|0x0c8|+0x073:'79'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}


} // namespace network
} // namespace vostok
