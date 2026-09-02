// SPDX-License-Identifier: GPL-3.0-or-later

#include "alpha.h"
#include "beta.h"

extern "C" int __cdecl entry( )
{
	alpha_use_type alpha = { 3 };
	beta_use_type beta = { 5 };
	return alpha.member + beta.member;
}
