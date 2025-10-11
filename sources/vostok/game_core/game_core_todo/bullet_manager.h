////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BULLET_MANAGER_H_INCLUDED
#define BULLET_MANAGER_H_INCLUDED

namespace survarium {

// STATE[STUB]
// survarium::bullet_manager::bullet_functor_mt_allocator::bullet_functor_mt_allocator(void* const, const unsigned int)
bullet_manager::bullet_functor_mt_allocator::bullet_functor_mt_allocator( void* buffer, u32 buffer_size )
{
	// LOCALS
	// bullet_manager::bullet_functor* e<1>
	// bullet_manager::bullet_functor* i<1>
	// ******

	// FUNCTION BODY
	// <0xbe481>|0x000|0x000:'161'
	// <0xbe48d>|0x00c|0x00c|[1]:'162'
	// <0xbe4bb>|0x03a|0x02e:'163'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::bullet_functor_mt_allocator::deallocate(survarium::bullet_manager::bullet_functor*&)
void bullet_manager::bullet_functor_mt_allocator::deallocate( bullet_manager::bullet_functor*& functor )
{
	// FUNCTION BODY
	// <0xbe53a>|0x000|0x000:'180'
	// <0xbe5a2>|0x068|0x068:'181'
	// ******
}

// STATE[STUB]
// void* survarium::bullet_manager::bullet_functor_mt_allocator::malloc_impl(unsigned int)
void* bullet_manager::bullet_functor_mt_allocator::malloc_impl( u32 size )
{
	return NULL;
	// FUNCTION BODY
	// 1
	// 2
	// <0xbe5c9>|0x000|0x000:'194'
	// <0xbe5ec>|0x023|0x023:'195'
	// ******
}

	/* TYPEDEFS

	typedef
		long
		counter_type;

	typedef
		vostok::memory::multi_threading_single_size_allocator_policy<vostok::memory::single_size_buffer_allocator<128,vostok::threading::simple_lock>::node>::free_list_type
		free_list_type;

} // namespace survarium

#endif // #ifndef BULLET_MANAGER_H_INCLUDED
