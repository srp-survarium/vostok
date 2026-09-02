// SPDX-License-Identifier: GPL-3.0-or-later

#include "alpha.h"
#include "beta.h"

extern "C" int __cdecl entry( )
{
	beta_use_type beta = { 5 };
	alpha_use_type alpha = { 3 };
	return beta.member + alpha.member;
}
