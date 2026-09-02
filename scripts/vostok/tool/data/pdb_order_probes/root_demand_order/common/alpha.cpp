// SPDX-License-Identifier: GPL-3.0-or-later

struct alpha_type
{
	int member;
};

extern "C" int __cdecl alpha_value( int value )
{
	alpha_type result = { value };
	return result.member + 1;
}
