////////////////////////////////////////////////////////////////////////////
//	Created		: 04.02.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/bone_names.h>
#include <vostok/animation/skeleton.h>

#include <boost/crc.hpp>

#ifndef	MASTER_GOLD
#include <vostok/configs_lua_config.h>
#endif // MASTER_GOLD

namespace vostok {
namespace animation {


bone_names::bone_names( ):
m_bone_count( u32(-1) ),
m_internal_memory_position ( size_t(-1) )
{

}

u32	bone_names::count_internal_memory_size	( u32 bones_count )
{
	return bones_count * sizeof( bone_name_index ); //excluding self memory
}

void bone_names::create_internals_in_place(u32 bones_count, void* memory  )
{
	R_ASSERT( memory );

	m_internal_memory_position = size_t ( pbyte( memory ) - pbyte( this ) );

	m_bone_count = bones_count;

	for ( u32 i = 0; i < bones_count; ++i )
	{
		bone_name_index * idx = bone_names_idx();
		new( &idx[i] ) bone_name_index();
	}
}

void bone_names::create_internals_in_place	( const bone_names &names, void* memory )
{
	create_internals_in_place( names.bones_number(), memory );
	for ( u32 i = 0; i < bones_number(); ++i )
	{
		bone_name_index * my_idx = bone_names_idx();
		bone_name_index const * in_idx = names.bone_names_idx();
		my_idx[ i ] = in_idx[ i ];
	}
}

void bone_names::create_internals_in_place	( configs::binary_config_ptr const& names, void* memory )
{
	configs::binary_config_value const& bones_names = (*names)["bones_names"];
	create_internals_in_place	( bones_names.size(), memory );

	bone_name_index* const indices = static_cast<bone_name_index*>(
		ALLOCA( bones_number() * sizeof(bone_name_index) )
	);
	for ( u32 n = 0; n < bones_number(); ++n )
	{
		strings::copy				( indices[n].name, static_cast<pcstr>( bones_names[n] ) );
		indices[n].index			= n;

		boost::crc_32_type processor;
		processor.process_block		( indices[n].name, indices[n].name + strings::length(indices[n].name) );
		indices[n].crc				= processor.checksum();
	}

	std::sort	( indices, indices + bones_number(), crc_compare_predicate() );

	for ( u32 n = 0; n < bones_number(); ++n )
		bone_names_idx()[n] = indices[n];
}

bone_name_index::bone_name_index(  u32 idx, pcstr aname ):
	index					( idx )
{
	boost::crc_32_type		processor;
	processor.process_block	( aname, aname + strings::length( aname ) );
	crc						= processor.checksum( );

	vostok::strings::copy		( name, aname );
}

bone_name_index::bone_name_index( ):
	index					( u32(-1) ),
	crc						(  u32(-1) )
{
}

void	bone_names::set_name( bone_index_type bone_index, pcstr name )
{
	bone_names_idx()[bone_index] = bone_name_index( bone_index, name );
}

void	bone_names::read( vostok::configs::binary_config_value const &cfg )
{
	const bone_index_type size = cfg.size();
	R_ASSERT( size == m_bone_count );

	bone_name_index * my_idx = bone_names_idx();

	for ( bone_index_type i = 0; i < size; ++i )
	{
		my_idx[i] = bone_name_index ( i, pcstr ( static_cast<pcstr>( cfg[i] ) ) ) ;
	}

	std::sort( my_idx, my_idx + m_bone_count, crc_compare_predicate( ) );
}

void	bone_names::write( vostok::configs::lua_config_value	&cfg )const
{

#ifndef	MASTER_GOLD
	const bone_index_type size = m_bone_count;
	for ( bone_index_type i = 0; i < size; ++i )
	{
		const bone_name_index &b = bone_names_idx()[i];
		cfg[ b.index ] = b.name;
		//cfg[i] = bone_names_idx()[i].c_str();
	}
#else
	VOSTOK_UNREFERENCED_PARAMETER					( cfg );
	NODEFAULT();
#endif // MASTER_GOLD
}

void	bone_names::write( stream &file )const
{
	const u32 size = m_bone_count;
	fwrite( &(size), sizeof(size), 1, file );
	fwrite( bone_names_idx(), sizeof(bone_name_index), size, file );
}

bone_index_type	bone_names::bone_index( pcstr name )const
{
	bone_name_index temp( u32(-1), name );

	vector<bone_name_index>::const_iterator r = std::lower_bound( bone_names_idx(), bone_names_idx() + m_bone_count, temp, crc_compare_predicate( ) );

	if ( temp.crc == r->crc &&
		name[ 0 ] == r->name[0]
		)
		return r->index;

	return u32(-1);
}

pcstr bone_names::bone_name		( bone_index_type index )const
{
	vector<bone_name_index>::const_iterator it = bone_names_idx();
	for (; it!=bone_names_idx() + m_bone_count; ++it)
		if (it->index == index)
			return it->name;

	return NULL;
}

} // namespace animation
} // namespace vostok
