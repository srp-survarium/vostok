// SPDX-License-Identifier: GPL-3.0-or-later

#include "shared.h"

typedef shared_type shared_type_alias;

struct beta_type
{
	int member;
};

extern "C" int __cdecl beta_value( int value )
{
	shared_type_alias shared = { value };
	beta_type beta = { 2 };
	return shared.member + beta.member;
}
