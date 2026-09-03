// SPDX-License-Identifier: GPL-3.0-or-later

extern "C" int __cdecl alpha_frame( int value );
extern "C" int __cdecl beta_frame( int value );

extern "C" int __cdecl entry( )
{
	return alpha_frame( 1 ) + beta_frame( 2 );
}
