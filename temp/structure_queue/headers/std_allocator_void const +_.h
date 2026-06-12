////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

namespace survarium {

class std_allocator< pcvoid > {
public:
	inline						std_allocator<void const *>	( ) { /* no source */ }

	inline	pcvoid*				address						( pcvoid& arg_0 ) const { /* no source */ }
	inline	pcvoid const*		address						( pcvoid const& arg_0 ) const { /* no source */ }

	inline	pcvoid*				allocate					( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	pcvoid*				allocate					( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	pcvoid*				_M_allocate					( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	pcvoid*				_M_allocate					( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*				__charalloc					( u32 arg_0 ) { /* no source */ }

	inline	void				deallocate					( pcvoid* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void				deallocate					( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void				construct					( pcvoid* arg_0, pcvoid const& arg_1 ) { /* no source */ }

	inline	void				destroy						( pcvoid* arg_0 ) { /* no source */ }

	inline	u32					max_size					( ) const { /* no source */ }
}; // class std_allocator< pcvoid >

STATIC_SIZE_ASSERT(std_allocator< pcvoid >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
