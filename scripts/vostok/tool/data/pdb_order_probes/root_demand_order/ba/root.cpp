// SPDX-License-Identifier: GPL-3.0-or-later

extern "C" int __cdecl alpha_value( int value );
extern "C" int __cdecl beta_value( int value );

extern "C" int __cdecl entry( )
{
	return beta_value( 5 ) + alpha_value( 3 );
}
