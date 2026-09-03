// SPDX-License-Identifier: GPL-3.0-or-later

extern "C" __declspec(noinline) int __cdecl beta_leaf( int value )
{
	return value + 2;
}

extern "C" __declspec(noinline) int __cdecl beta_entry( int value )
{
	return beta_leaf( value ) + 20;
}
