// SPDX-License-Identifier: GPL-3.0-or-later

#include "beta.h"
#include "alpha.h"

extern "C" int __cdecl entry( )
{
	alpha_header_type const alpha = { 3 };
	beta_header_type const beta = { 5 };
	return alpha_header_value( alpha ) + beta_header_value( beta );
}
