////////////////////////////////////////////////////////////////////////////
//	Created 	: 11.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <vostok/network/api.h>

#include "std_string.h"

namespace vostok {
namespace network {

extern allocator_type*						g_allocator;

#define USER_ALLOCATOR						*::vostok::network::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

template < typename T >
class vector_size_t : public vector< T > {
private:
	typedef vector< T >						super;

public:
	typedef vector_size_t< T >				self_type;
	typedef typename super::size_type		size_type;
	typedef typename super::value_type		value_type;
	typedef typename super::reference		reference;
	typedef typename super::const_reference	const_reference;

public:
	inline				vector_size_t		( ) {}
	inline				vector_size_t		( size_type count, value_type const& value ) : super(count,value) {}
	inline				vector_size_t		( self_type const& other) : super(other) {}
	inline	explicit	vector_size_t		( size_type count) : super(count) {}
	template < typename input_iterator >
	inline				vector_size_t		( input_iterator const& first, input_iterator const& last ) : super(first, last) {}
	inline	reference		operator[]		( size_t index ) { return super::operator[]( (u32)index ); }
	inline	const_reference	operator[]		( size_t index ) const { return super::operator[]( (u32)index ); }
}; // class vector

} // namespace network
} // namespace vostok

#define XN_NEW( type )								VOSTOK_NEW_IMPL(		*::vostok::network::g_allocator, type )
#define XN_DELETE( pointer )						VOSTOK_DELETE_IMPL(	*::vostok::network::g_allocator, pointer )
#define XN_MALLOC( size, description )				VOSTOK_MALLOC_IMPL(	*::vostok::network::g_allocator, size, description )
#define XN_REALLOC( pointer, size, description )	VOSTOK_REALLOC_IMPL(	*::vostok::network::g_allocator, pointer, size, description )
#define XN_FREE( pointer )							VOSTOK_FREE_IMPL(		*::vostok::network::g_allocator, pointer )
#define XN_ALLOC( type, count )						VOSTOK_ALLOC_IMPL(	*::vostok::network::g_allocator, type, count )

#define VOSTOK_BOOST_NEW								XN_NEW
#define VOSTOK_BOOST_DELETE							XN_DELETE

#define VOSTOK_BOOST_MALLOC							XN_MALLOC
#define VOSTOK_BOOST_FREE								XN_FREE

#endif // #ifndef MEMORY_H_INCLUDED