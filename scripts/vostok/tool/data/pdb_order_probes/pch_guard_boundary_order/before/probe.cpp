// SPDX-License-Identifier: GPL-3.0-or-later

#include "shared.h"
#include "pch.h"

int guard_boundary_anchor( guard_boundary_shared_type value );

extern "C" int __cdecl entry( )
{
	guard_boundary_shared_type const value = { 3 };
	return guard_boundary_anchor( value );
}
