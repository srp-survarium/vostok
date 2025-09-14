////////////////////////////////////////////////////////////////////////////
//	Created		: 21.09.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef TYPE_UID_OBJECT_H_INCLUDED
#define TYPE_UID_OBJECT_H_INCLUDED

#include <vostok/threading_functions.h>
namespace vostok {

template <class SubClass>
class uid_object 
{
public:
	uid_object()
	{
		m_uid	=	threading::interlocked_increment(s_uid);
		m_uid	=	m_uid;
	}

	u32				uid	() const { return (u32)m_uid; }

private:
	u32								m_uid;
	static threading::atomic32_type	s_uid;
}; // class type_uid_object

template <class SubClass>
threading::atomic32_type	uid_object<SubClass>::s_uid	=	0;

} // namespace vostok

#endif // #ifndef TYPE_UID_OBJECT_H_INCLUDED