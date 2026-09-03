// SPDX-License-Identifier: GPL-3.0-or-later

#include "item.h"

void __cdecl operator delete( void* )
{
}

void __cdecl operator delete[]( void* )
{
}

probe_item::~probe_item( )
{
}

probe_item* scalar_item = 0;
probe_item* vector_item = 0;
