// SPDX-License-Identifier: GPL-3.0-or-later

extern "C" __declspec(noinline) int __cdecl alpha_leaf( int value )
{
	return value + 1;
}

extern "C" __declspec(noinline) int __cdecl alpha_entry( int value )
{
	return alpha_leaf( value ) + 10;
}
