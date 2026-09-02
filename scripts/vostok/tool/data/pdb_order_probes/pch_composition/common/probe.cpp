// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "shared.h"

int pch_anchor( pch_shared_type value );

extern "C" int __cdecl entry( )
{
	pch_shared_type const value = { 3 };
	return pch_anchor( value );
}
