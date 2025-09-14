////////////////////////////////////////////////////////////////////////////
//	Created 	: 16.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOOSE_PTR_BASE_H_INCLUDED
#define VOSTOK_LOOSE_PTR_BASE_H_INCLUDED

#include <vostok/loose_ptr_data.h>

namespace vostok {

class loose_ptr_base {
public:
	inline			loose_ptr_base	( );
	inline			~loose_ptr_base	( );

	loose_ptr_data*	m_pointer;
}; // struct loose_ptr_base

} // namespace vostok

#include <vostok/loose_ptr_base_inline.h>

#endif // #ifndef VOSTOK_LOOSE_PTR_BASE_H_INCLUDED