// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HIT_TYPE_PARAMETERS_MODIFYER_H_INCLUDED
#define HIT_TYPE_PARAMETERS_MODIFYER_H_INCLUDED

namespace survarium {

struct hit_type_parameters_modifyer {
	/* 0x0000 */	float		armor;
	/* 0x0004 */	float		reduce;
	/* 0x0008 */	float		absorption;
}; // struct hit_type_parameters_modifyer

STATIC_SIZE_ASSERT(hit_type_parameters_modifyer, 0xC);

} // namespace survarium

#endif // #ifndef HIT_TYPE_PARAMETERS_MODIFYER_H_INCLUDED
