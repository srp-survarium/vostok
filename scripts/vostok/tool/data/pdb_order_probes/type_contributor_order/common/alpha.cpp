// SPDX-License-Identifier: GPL-3.0-or-later

#include "shared.h"

typedef shared_type shared_type_alias;

struct alpha_type
{
	int member;
};

extern "C" int __cdecl alpha_value( int value )
{
	shared_type_alias shared = { value };
	alpha_type alpha = { 1 };
	return shared.member + alpha.member;
}
