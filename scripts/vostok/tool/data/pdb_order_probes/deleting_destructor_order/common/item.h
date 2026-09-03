// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DELETING_DESTRUCTOR_ORDER_ITEM_H_INCLUDED
#define DELETING_DESTRUCTOR_ORDER_ITEM_H_INCLUDED

class probe_item
{
public:
	virtual ~probe_item( );
};

extern probe_item* scalar_item;
extern probe_item* vector_item;

#endif // #ifndef DELETING_DESTRUCTOR_ORDER_ITEM_H_INCLUDED
