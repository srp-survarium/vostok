////////////////////////////////////////////////////////////////////////////
//	Created		: 07.12.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_SKELETON_H_INCLUDED
#define VOSTOK_ANIMATION_SKELETON_H_INCLUDED

#include <vostok/animation/type_definitions.h>
#include <vostok/animation/api.h>
#include <vostok/animation/skeleton_bone.h>

namespace vostok {
namespace animation {

class skeleton_bone;

class VOSTOK_ANIMATION_API skeleton : public resources::unmanaged_resource {
public:
	explicit						skeleton		( u32 bones_count );
									~skeleton		( );
			pcstr					get_bone_name	( bone_index_type index ) const;
			bone_index_type			get_bone_index	( pcstr name ) const;
	inline	u32						get_bone_index	( skeleton_bone const& bone ) const;
	inline	u32						get_bones_count	( ) const;
	inline	u32					get_root_bones_count( ) const;
	inline	u32				get_non_root_bones_count( ) const;
	inline	skeleton_bone const&	get_bone		( bone_index_type index ) const;
	inline	skeleton_bone const&	get_root		( ) const;
	inline	bool					is_root_bone	( bone_index_type index ) const;

private:
	inline	skeleton_bone&			get_root		( );


private:
	bone_index_type					m_bones_count;
}; // class skeleton

typedef	resources::resource_ptr<
	skeleton,
	resources::unmanaged_intrusive_base
> skeleton_ptr;

	bone_index_type			skeleton_bone_index	( skeleton const& skeleton, pcstr bone_name );

} // namespace animation
} // namespace vostok

#include <vostok/animation/skeleton_inline.h>

#endif // #ifndef VOSTOK_ANIMATION_SKELETON_H_INCLUDED
