// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LEG_INFO_H_INCLUDED
#define LEG_INFO_H_INCLUDED

namespace survarium {

/* survarium::leg_info */

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

struct leg_info {
public:
	/* 0x0000 */	u32			hip_index;
	/* 0x0004 */	u32			ankle_index;
	/* 0x0008 */	u32			toe_index;
	/* 0x000c */	float3		ankle_heel_vector;
	/* 0x0018 */	float3		toe_toetip_vector;
}; // struct leg_info

STATIC_SIZE_ASSERT(leg_info, 0x24);

} // namespace survarium

#endif // #ifndef LEG_INFO_H_INCLUDED
