// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "wrapper.h"

extern "C" int __cdecl entry( )
{
	alpha_post_pch_type const alpha = { 3 };
	beta_post_pch_type const beta = { 5 };
	return alpha_post_pch_value( alpha ) + beta_post_pch_value( beta );
}
