// SPDX-License-Identifier: GPL-3.0-or-later

// checksum content marker: changed!
#include "value.h"

extern "C" int __cdecl entry( )
{
	checksum_content_value const value = { 3 };
	return checksum_header_value( value );
}
