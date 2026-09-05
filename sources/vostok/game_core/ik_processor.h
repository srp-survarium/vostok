// SPDX-License-Identifier: GPL-3.0-or-later

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

	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	animation::skeleton const*		m_skeleton;
	/* 0x0004 */	u32								m_last_time_in_ms;
}; // class ik_processor

STATIC_SIZE_ASSERT(ik_processor, 0x8);

} // namespace survarium

#endif // #ifndef IK_PROCESSOR_H_INCLUDED
