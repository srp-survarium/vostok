////////////////////////////////////////////////////////////////////////////
//	Created		: 10.08.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SIZE_POLICY_H_INCLUDED
#define VOSTOK_SIZE_POLICY_H_INCLUDED

namespace vostok {

class VOSTOK_CORE_API no_size_policy
{
public:
	void			increment_size	() {}
	void			decrement_size	() {}
	void			set_zero_size	() {}
	u32				size			() const { NOT_IMPLEMENTED(return 0); }
	void			swap			(no_size_policy &) {}
};

class VOSTOK_CORE_API size_policy
{
public:
					size_policy		() : m_size(0) {}
	u32				size			() const { return m_size; }
	void			swap			(size_policy & other) { std::swap(m_size, other.m_size); }

protected:
	void			increment_size	() { ++m_size; }
	void			decrement_size	() { --m_size; }
	void			set_zero_size	() { m_size	=	0; }

private:
	u32				m_size;
};

} // namespace vostok

#endif // #ifndef VOSTOK_SIZE_POLICY_H_INCLUDED