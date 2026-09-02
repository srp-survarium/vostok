// SPDX-License-Identifier: GPL-3.0-or-later

#include "shared.h"
#include "pch.h"

int boundary_anchor( boundary_shared_type value );

extern "C" int __cdecl entry( )
{
	boundary_shared_type const value = { 3 };
	return boundary_anchor( value );
}
