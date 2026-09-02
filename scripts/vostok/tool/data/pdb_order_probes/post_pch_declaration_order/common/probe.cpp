// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "wrapper.h"

extern "C" int __cdecl entry( )
{
	declaration_value const value = { 3, declaration_beta };
	return definition_value( value.member ) + value.kind;
}
