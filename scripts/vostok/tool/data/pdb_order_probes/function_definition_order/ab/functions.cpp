// SPDX-License-Identifier: GPL-3.0-or-later

extern "C" __declspec(noinline) int __cdecl alpha_frame( int value )
{
	volatile int slots[3];
	slots[0] = value;
	slots[1] = value + 1;
	slots[2] = value + 2;
	return slots[0] + slots[1] + slots[2];
}

extern "C" __declspec(noinline) int __cdecl beta_frame( int value )
{
	volatile int slots[7];
	slots[0] = value;
	slots[6] = value + 6;
	return slots[0] + slots[6];
}
