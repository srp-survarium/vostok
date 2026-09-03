// SPDX-License-Identifier: GPL-3.0-or-later

extern "C" int __cdecl alpha_entry( int value );
extern "C" int __cdecl beta_entry( int value );

extern "C" int __cdecl entry( )
{
	return alpha_entry( 3 ) + beta_entry( 5 );
}
