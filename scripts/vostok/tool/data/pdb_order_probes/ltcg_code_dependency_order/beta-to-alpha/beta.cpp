// SPDX-License-Identifier: GPL-3.0-or-later

extern "C" int __cdecl alpha_leaf( int value );

extern "C" __declspec(noinline) int __cdecl beta_leaf( int value )
{
	return value + 2;
}

extern "C" __declspec(noinline) int __cdecl beta_entry( int value )
{
	return alpha_leaf( value ) + 20;
}
