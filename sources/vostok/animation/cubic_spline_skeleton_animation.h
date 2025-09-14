////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_CUBIC_SPLINE_SKELETON_ANIMATION_H_INCLUDED
#define VOSTOK_ANIMATION_CUBIC_SPLINE_SKELETON_ANIMATION_H_INCLUDED

#include <vostok/animation/bone_animation.h>
#include <vostok/animation/skeleton.h>
#include <vostok/animation/bone_names.h>
#include <vostok/animation/animation_event_channels.h>
#include <vostok/animation/type_definitions.h>

#ifndef	MASTER_GOLD
#	include <vostok/animation/editable_animation_event_channels.h>
#endif // #ifndef MASTER_GOLD

namespace vostok {
namespace animation {

class skeleton;
class bi_spline_skeleton_animation;

class VOSTOK_ANIMATION_API cubic_spline_skeleton_animation {
public:
	static	u32								count_memory_size	( const bi_spline_skeleton_animation &data );
#ifndef MASTER_GOLD
	static	cubic_spline_skeleton_animation* new_animation		( memory::base_allocator& allocator, bi_spline_skeleton_animation const& animation );
#endif // #ifndef MASTER_GOLD
	static	cubic_spline_skeleton_animation* new_animation		( pvoid buffer_for_animation, bi_spline_skeleton_animation const& animation );
	static	void							 delete_animation	( cubic_spline_skeleton_animation const*& animation_to_delete );

public:
	inline	bone_animation	const&	bone				( u32 id ) const	{ return bone_data()[id]; }
	inline	bone_animation&			bone				( u32 id )			{ return bone_data()[id]; }

	inline	bone_animation	const&	bone				( pcstr id ) const;
	inline	bone_animation&			bone				( pcstr id );

	inline	bool					has_bone			( pcstr id ) const;
	inline	float					get_frames_per_second( ) const { return 30.f; }
//	inline	bool					has_bone			( u32 id ) const;

public:
			u32						bones_count			( ) const { return m_bone_count; }
	inline	float					max_time_in_frames	( ) const { return bone_data()[0].max_time(); }
	inline	float					min_time_in_frames	( ) const { return bone_data()[0].min_time(); }
	inline	float					length_in_frames	( ) const { return max_time_in_frames() - min_time_in_frames(); }
	inline	animation_types_enum	animation_type		( ) const { return m_animation_type; }

public:
			bone_names const&		get_bone_names		( ) const { return m_bone_names; }
	
	inline	animation_event_channels const& event_channels		( ) const;		

private:
						cubic_spline_skeleton_animation	( bi_spline_skeleton_animation const& animation );
						~cubic_spline_skeleton_animation( );
			void			create_in_place_internals	( const bi_spline_skeleton_animation &data, void *memory );
			bone_animation*			bone_data			( )			{ return (bone_animation*) ( pbyte(this) + m_internal_memory_position ); }
			bone_animation const*	bone_data			( ) const	{ return (bone_animation*) ( pbyte(this) + m_internal_memory_position ); }

private:
	bone_names						m_bone_names;
	animation_event_channels		m_event_channels;
	u32								m_bone_count;
	u32								m_internal_memory_position;
	animation_types_enum			m_animation_type;

#ifndef MASTER_GOLD
public:
		void	set_file_name		( fs_new::virtual_path_string const & name ) { m_file_name = name; };
		fs_new::virtual_path_string	get_file_name		( ) const { return m_file_name; }
	inline	editable_animation_event_channels const*	get_editable_animation_event_channels	( ) const	{ return m_editable_event_channels; }

private:
	fs_new::virtual_path_string					m_file_name;
	mutable	editable_animation_event_channels*	m_editable_event_channels;
	memory::base_allocator*			m_allocator;
#endif // #ifndef MASTER_GOLD

}; // class cubic_spline_skeleton_animation

} // namespace animation

namespace resources {
	template class VOSTOK_ANIMATION_API pinned_ptr_const< animation::cubic_spline_skeleton_animation >;
} // namespace resources

namespace animation {

typedef resources::pinned_ptr_const< cubic_spline_skeleton_animation > cubic_spline_skeleton_animation_pinned;

} // namespace animation
} // namespace vostok

#include <vostok/animation/cubic_spline_skeleton_animation_inline.h>

#endif // #ifndef VOSTOK_ANIMATION_CUBIC_SPLINE_SKELETON_ANIMATION_H_INCLUDED