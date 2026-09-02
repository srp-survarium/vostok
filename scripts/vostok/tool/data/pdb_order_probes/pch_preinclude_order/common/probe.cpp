// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"

int pch_preinclude_anchor( preincluded_type value );

extern "C" int __cdecl entry( )
{
	preincluded_type const value = { 3, preincluded_beta };
	return pch_preinclude_anchor( value );
}
