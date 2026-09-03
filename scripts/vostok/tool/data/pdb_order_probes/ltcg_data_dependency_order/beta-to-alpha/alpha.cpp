// SPDX-License-Identifier: GPL-3.0-or-later

extern "C" volatile int alpha_data = 1;

extern "C" __declspec(noinline) int __cdecl alpha_entry( int value )
{
	return value + alpha_data;
}
