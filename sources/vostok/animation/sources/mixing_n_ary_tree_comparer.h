////////////////////////////////////////////////////////////////////////////
//	Created		: 04.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef MIXING_N_ARY_TREE_COMPARER_H_INCLUDED
#define MIXING_N_ARY_TREE_COMPARER_H_INCLUDED

#include <vostok/animation/base_interpolator.h>

namespace vostok {
namespace animation {

struct base_interpolator;
class bone_mixer;
class interpolator_size_calculator;

namespace mixing {

class n_ary_tree;
struct binary_tree_base_node;

struct n_ary_tree_base_node;
class n_ary_tree_animation_node;

struct animation_state;
class animated_object_holder;

class n_ary_tree_size_calculator;

class n_ary_tree_comparer {
public:
	friend class n_ary_tree_size_calculator;
	friend class ::vostok::animation::interpolator_size_calculator;

					n_ary_tree_comparer			(
						n_ary_tree const& from,
						n_ary_tree const& to,
						u32 current_time_in_ms
					);
			bool	equal						( ) const;
			u32		needed_buffer_size			( ) const;
	inline	u32		animations_count			( ) const { return m_animations_count; }
	inline	u32		animated_objects_count		( ) const { return m_animated_objects_count; }

private:
					n_ary_tree_comparer			( n_ary_tree_comparer const& );
	n_ary_tree_comparer& operator =				( n_ary_tree_comparer const& );

private:
			void	process_animations			(
						n_ary_tree_animation_node*& i,
						n_ary_tree_animation_node*& j
					);

private:
	template < typename T >
	inline	void	increase_buffer_size		( T& node );
			void	increase_buffer_size		( n_ary_tree_base_node& node );

			bool	new_time_scale						( n_ary_tree_animation_node& new_time_driving_animation );
			void	new_animation						(
						n_ary_tree_animation_node&		animation,
						u32&							time_scale_operands_count,
						u32&							operands_offset
					);
			void	new_weight_transition				( float from, float to );
			void	new_weight_transition				( n_ary_tree_base_node& from, n_ary_tree_base_node& to );
			void	new_weight_transition				(
						base_interpolator const&	from_animation_interpolator,
						n_ary_tree_base_node&		from,
						float						to
					);
			void	new_weight_transition				(
						base_interpolator const&	to_animation_interpolator,
						float						from,
						n_ary_tree_base_node&		to
					);
			void	new_time_scale_transition			( n_ary_tree_base_node& from, n_ary_tree_base_node& to );
			void	new_time_scale_transition			( n_ary_tree_base_node& from, float to );
			void	new_time_scale_transition			( float from, n_ary_tree_base_node& to );

			void	add_operands						(
						n_ary_tree_animation_node&		from,
						n_ary_tree_animation_node&		to,
						const bool						skip_time_scale_node
					);
			void	add_animation						(
						n_ary_tree_animation_node&			animation,
						n_ary_tree_animation_node* const	weight_driving_animation
					);
			void	remove_animation					(
						n_ary_tree_animation_node&			animation,
						n_ary_tree_animation_node const*	weight_driving_animation,
						bool								is_new_driving_animation
					);
			void	change_animation					(
						n_ary_tree_animation_node&		from,
						n_ary_tree_animation_node&		to,
						bool							is_new_driving_animation
					);

			void	merge_weight_asynchronous_groups	(
						n_ary_tree_animation_node* const	from_begin,
						n_ary_tree_animation_node* const	from_end,
						n_ary_tree_animation_node* const	to_begin,
						n_ary_tree_animation_node* const	to_end
					);
			void	merge_weight_synchronization_groups	(
						n_ary_tree_animation_node*		from_begin,
						n_ary_tree_animation_node*		from_end,
						n_ary_tree_animation_node*		to_begin,
						n_ary_tree_animation_node*		to_end,
						n_ary_tree_animation_node&		new_weight_driving_animation,
						bool							is_new_driving_animation
					);

			void	new_weight_driving_animation		( n_ary_tree_animation_node& animation );
			void	new_weight_driving_animation		(
						n_ary_tree_animation_node&		new_weight_driving_animation,
						n_ary_tree_animation_node&		new_driving_animation_in_previous_target
					);

			void	add_weight_synchronization_group	( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end );
			void	remove_weight_synchronization_group	( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end );
			void	change_weight_synchronization_group	(
						n_ary_tree_animation_node*		from_begin,
						n_ary_tree_animation_node*		from_end,
						n_ary_tree_animation_node*		to_begin,
						n_ary_tree_animation_node*		to_end
					);

			void	merge_trees							( n_ary_tree const& from, n_ary_tree const& to );

			void	process_interpolators				( n_ary_tree const& from, n_ary_tree const& to );

	inline	void	advance_buffer						( const u32 size ) { m_needed_buffer_size += size; }
private:
	/* 0x0000 */	animated_object_holder*		m_animated_objects;
	/* 0x0004 */	animated_object_holder*		m_animated_objects_end;
	/* 0x0008 */	n_ary_tree const&			m_from;
	/* 0x000c */	n_ary_tree const&			m_to;
	/* 0x0010 */	u32							m_animations_count;
	/* 0x0014 */	u32							m_animated_objects_count;
	/* 0x0018 */	u32							m_needed_buffer_size;
	/* 0x001c */	const u32					m_current_time_in_ms;
	/* 0x0020 */	bool						m_equal;
}; // class n_ary_tree_comparer

STATIC_SIZE_ASSERT(n_ary_tree_comparer, 0x24);

class animation_comparer_predicate {
public:
	inline			animation_comparer_predicate( const bool use_synchronized_animations, const bool use_overriding_animations ) :
						m_use_synchronized_animations	( use_synchronized_animations ),
						m_use_overriding_animations		( use_overriding_animations )  { }

			comparison_result_enum	operator()			( n_ary_tree_animation_node const& left, n_ary_tree_animation_node const& right ) const;
private:
	/* 0x0000 */	const bool		m_use_synchronized_animations;
	/* 0x0001 */	const bool		m_use_overriding_animations;
}; // class animation_comparer_predicate

class animation_comparer_less_predicate {
public:
	inline			animation_comparer_less_predicate	( const bool use_synchronized_animations, const bool use_overriding_animations ) :
						m_predicate( use_synchronized_animations, use_overriding_animations ) { }

	inline	bool	operator()							( n_ary_tree_animation_node const& left, n_ary_tree_animation_node const& right ) const { return m_predicate( left, right ) == less; }
	inline	bool	operator()							( n_ary_tree_animation_node const* const left, n_ary_tree_animation_node const* const right ) const { return (*this)( *left, *right ); }

private:
	/* 0x0000 */	animation_comparer_predicate	m_predicate;
}; // class animation_comparer_less_predicate

class animation_comparer_equal_predicate {
public:
	inline			animation_comparer_equal_predicate	( const bool use_synchronized_animations, const bool use_overriding_animations ) :
						m_predicate( use_synchronized_animations, use_overriding_animations ) { }

	inline	bool	operator()							( n_ary_tree_animation_node const& left, n_ary_tree_animation_node const& right ) const { return m_predicate( left, right ) == equal; }

private:
	/* 0x0000 */	animation_comparer_predicate	m_predicate;
}; // class animation_comparer_equal_predicate

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef MIXING_N_ARY_TREE_COMPARER_H_INCLUDED
