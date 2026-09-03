// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "shared.h"

extern "C" int __cdecl alpha_value( );
extern "C" int __cdecl beta_value( );

extern "C" int __cdecl entry( )
{
	batch_shared_type const value = { 7 };
	return value.member + alpha_value( ) + beta_value( );
}
