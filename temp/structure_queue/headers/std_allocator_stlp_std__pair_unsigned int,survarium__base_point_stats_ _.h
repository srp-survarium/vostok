////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::pair<unsigned int,survarium::base_point_stats>;

namespace survarium {

class std_allocator< std::pair< u32, base_point_stats > > {
public:
	inline											std_allocator<stlp_std::pair<unsigned int,survarium::base_point_stats> >( ) { /* no source */ }

	inline	std::pair< u32, base_point_stats >*		address		( std::pair< u32, base_point_stats >& arg_0 ) const { /* no source */ }
	inline	std::pair< u32, base_point_stats > const*	address		( std::pair< u32, base_point_stats > const& arg_0 ) const { /* no source */ }

	inline	std::pair< u32, base_point_stats >*		allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< u32, base_point_stats >*		allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::pair< u32, base_point_stats >*		_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< u32, base_point_stats >*		_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*									__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void									deallocate	( std::pair< u32, base_point_stats >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void									deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void									construct	(
														std::pair< u32, base_point_stats >*		arg_0,
														std::pair< u32, base_point_stats > const&	arg_1
													) { /* no source */ }

	inline	void									destroy		( std::pair< u32, base_point_stats >* arg_0 ) { /* no source */ }

	inline	u32										max_size	( ) const { /* no source */ }
}; // class std_allocator< std::pair< u32, base_point_stats > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< u32, base_point_stats > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
