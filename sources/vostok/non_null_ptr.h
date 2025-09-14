////////////////////////////////////////////////////////////////////////////
//	Created 	: 16.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_NON_NULL_PTR_H_INCLUDED
#define VOSTOK_NON_NULL_PTR_H_INCLUDED

#if VOSTOK_USE_DEBUG_POINTERS

#include <vostok/raw_ptr.h>

namespace vostok {

template < typename type, typename pointer_type = type* >
class non_null_ptr : public raw_ptr< type, pointer_type > {
public:
	inline	IMPLICIT	non_null_ptr( pointer_type value );

	template < typename type2, typename pointer_type2 >
	inline	IMPLICIT	non_null_ptr( raw_ptr< type2, pointer_type2 > const& value );

	template < typename type2, typename pointer_type2 >
	inline	IMPLICIT	non_null_ptr( non_null_ptr< type2, pointer_type2 > const& value );

	template < typename type2, typename base_type, typename threading_policy >
	inline	IMPLICIT	non_null_ptr( loose_ptr< type2, base_type, threading_policy > const& value );

private:
	typedef raw_ptr< type, pointer_type >	super;

private:
	inline	IMPLICIT	non_null_ptr( int value );
};

} // namespace vostok

#include <vostok/non_null_ptr_inline.h>

	namespace vostok {
		template < typename T, typename T_pointer = T* >
		struct non_null {
			typedef non_null_ptr< T, T_pointer >	ptr;
		}; // struct non_null
	} // namespace vostok
#else // #if VOSTOK_USE_DEBUG_POINTERS
	namespace vostok {
		template < typename T, typename T_pointer = T* >
		struct non_null {
			typedef T*								ptr;
		}; // struct non_null
	} // namespace vostok
#endif // #if VOSTOK_USE_DEBUG_POINTERS

#endif // #ifndef VOSTOK_NON_NULL_PTR_H_INCLUDED