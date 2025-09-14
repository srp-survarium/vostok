////////////////////////////////////////////////////////////////////////////
//	Created		: 04.02.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_BONE_NAMES_H_INCLUDED
#define VOSTOK_ANIMATION_BONE_NAMES_H_INCLUDED

#include <vostok/shared_string.h>
#include <vostok/animation/type_definitions.h>

namespace vostok {

namespace configs{
	class lua_config_value;
} // namespace configs

namespace animation {

class skeleton;
typedef string64	bone_name_string_type;

struct bone_name_index {

	bone_name_index( u32 idx, pcstr name );
	bone_name_index( );

	bone_name_string_type name;
	u32	crc;
	u32 index;
};

class VOSTOK_ANIMATION_API bone_names:
	private core::noncopyable
{
public:
	
				bone_names( );

static		u32	count_internal_memory_size	( u32 m_bone_count );

public:
			void			create_internals_in_place	( u32 bones_count, void* memory );
			void			create_internals_in_place	( const bone_names &names, void* memory );
			
			u32				bones_number	( ) const { return m_bone_count; }


			void			set_name		( bone_index_type bone_index, pcstr name );

public:	
			void			read			( vostok::configs::binary_config_value const &cfg );
			void			write			( vostok::configs::lua_config_value		&cfg )const;

			void			write			( stream &file )const;

public:
			void			create_index	( const skeleton &skel, vector< bone_index_type > &index )const;

public:
			bone_index_type	bone_index		( pcstr name )const;
			pcstr			bone_name		( bone_index_type index )const;

private:
	bone_name_index			*bone_names_idx() { return static_cast<bone_name_index*> (  pvoid(  pbyte( this ) + m_internal_memory_position ) ); }

public:
	bone_name_index	const	*bone_names_idx() const  {  return  static_cast<bone_name_index*> ( pvoid(pbyte( this ) + m_internal_memory_position) ); }
	
private:
	size_t					m_internal_memory_position;
	u32						m_bone_count;
}; // class bone_names

} // namespace animation
} // namespace vostok

#endif // #ifndef VOSTOK_ANIMATION_BONE_NAMES_H_INCLUDED