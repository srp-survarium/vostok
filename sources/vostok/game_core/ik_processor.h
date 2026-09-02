// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef IK_PROCESSOR_H_INCLUDED
#define IK_PROCESSOR_H_INCLUDED

namespace vostok {
namespace animation {
	class skeleton;
}
}

namespace survarium {

class ik_processor : private core::noncopyable {
protected:
			explicit	ik_processor	( );
			void		activate		( animation::skeleton const& skeleton );

	// claude@NOTE: legs_ik_processor reads m_skeleton DIRECTLY (protected): every target
	// call site flows the member load straight into the consumer with NO reference temp,
	// while a get_skeleton() inline accessor leaves a /Od call-result temp (+0xc per site).
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	animation::skeleton const*		m_skeleton;
	// claude@NOTE: legs_ik_processor::tick reads/writes m_last_time_in_ms directly
	// (protected so the derived class can touch it; protected vs private emits no bytes).
	/* 0x0004 */	u32								m_last_time_in_ms;
}; // class ik_processor

STATIC_SIZE_ASSERT(ik_processor, 0x8);

} // namespace survarium

#endif // #ifndef IK_PROCESSOR_H_INCLUDED
