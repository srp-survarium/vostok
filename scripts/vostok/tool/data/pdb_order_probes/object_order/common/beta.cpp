// SPDX-License-Identifier: GPL-3.0-or-later

struct beta_type
{
	int member;
};

extern "C" int __cdecl beta_value( int value )
{
	beta_type result = { value };
	return result.member + 2;
}
