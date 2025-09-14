////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2009
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_DEBUG_CHECK_INVARIANTS
#define VOSTOK_DEBUG_CHECK_INVARIANTS

// instructions:
// add check_invariants () const to your class
// put VOSTOK_CHECK_INVARIANTS(your_class) in class functions

#ifdef MASTER_GOLD
#	define VOSTOK_CHECK_INVARIANTS 
#else // #ifdef MASTER_GOLD
#	define VOSTOK_CHECK_INVARIANTS(Type)	vostok::debug::detail::check_invariants_raii<Type> \
																check_invariants_helper$	(this)
#endif // #ifdef MASTER_GOLD 

namespace vostok {
namespace debug {
namespace detail {

template <class T>
class check_invariants_raii
{
public:
	template <class T1>
	check_invariants_raii (T1 const * const object)
	{
		m_object					=	object;
		m_object->check_invariants		();
	}

	~check_invariants_raii ()
	{
		m_object->check_invariants		();
	}

private:
	T const *							m_object;
};

} // namespace detail
} // namespace debug
} // namespace vostok

#endif // #ifndef VOSTOK_DEBUG_CHECK_INVARIANTS