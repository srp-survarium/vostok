// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ARTEFACT_BASE_H_INCLUDED
#define ARTEFACT_BASE_H_INCLUDED

#include <vostok/game_core/inventory_item.h>

namespace survarium {

class artefact_base : public inventory_item {
public:
	inline			artefact_base	( ) : inventory_item( inventory_item::use_silent ) { }
}; // class artefact_base

STATIC_SIZE_ASSERT(artefact_base, 0x118);

typedef resources::resource_ptr< artefact_base, resources::unmanaged_intrusive_base > artefact_base_ptr;

} // namespace survarium

#endif // #ifndef ARTEFACT_BASE_H_INCLUDED
