////////////////////////////////////////////////////////////////////////////
//	Created		: 07.12.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/skeleton.h>

using vostok::animation::skeleton_bone;
using vostok::animation::bone_index_type;
using vostok::animation::skeleton;
using vostok::strings::compare;

skeleton::skeleton			( u32 const bones_count ) :
	m_bones_count			( bones_count )
{
}

skeleton::~skeleton			( )
{
	skeleton_bone* i		= &get_root();
	skeleton_bone* const e	= i + m_bones_count;
	for ( ; i != e; ++i )
		i->~skeleton_bone	( );
}

struct bone_id_predicate {
	inline	bone_id_predicate	( pcstr const bone_name ) :
		m_bone_name			( bone_name )
	{
	}

	inline bool operator ( )	( skeleton_bone const& skeleton_bone )
	{
		return				vostok::strings::equal( skeleton_bone.id(), m_bone_name );
	}

	pcstr					m_bone_name;
}; // struct bone_id_predicate

bone_index_type vostok::animation::skeleton_bone_index	( skeleton const& skeleton, pcstr const bone_name )
{
	skeleton_bone const* const begin	= &skeleton.get_root();

	return					std::find_if( begin, begin + skeleton.get_bones_count(), bone_id_predicate( bone_name ) ) - begin;
}

bone_index_type	skeleton::get_bone_index	( pcstr const name ) const
{
	return					skeleton_bone_index( *this, name );
}
