// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"

int pch_anchor( alpha_pch_type alpha, beta_pch_type beta );

extern "C" int __cdecl entry( )
{
	alpha_pch_type const alpha = { 3 };
	beta_pch_type const beta = { 5 };
	return pch_anchor( alpha, beta );
}
