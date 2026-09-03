// SPDX-License-Identifier: GPL-3.0-or-later

extern "C" volatile int alpha_data;
extern "C" volatile int beta_data = 2;

extern "C" __declspec(noinline) int __cdecl beta_entry( int value )
{
	return value + alpha_data;
}
