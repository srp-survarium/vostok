// SPDX-License-Identifier: GPL-3.0-or-later

extern "C" int __cdecl alpha_value( int value );

extern "C" int __cdecl entry( )
{
	return alpha_value( 3 );
}
