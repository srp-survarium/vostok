// SPDX-License-Identifier: GPL-3.0-or-later

#include "item.h"

extern "C" void __cdecl entry( )
{
	delete scalar_item;
	delete[] vector_item;
}
