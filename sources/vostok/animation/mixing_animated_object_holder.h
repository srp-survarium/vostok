////////////////////////////////////////////////////////////////////////////
//	Created 	: 23.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_MIXING_ANIMATED_OBJECT_HOLDER_H_INCLUDED
#define ANIMATION_MIXING_ANIMATED_OBJECT_HOLDER_H_INCLUDED

namespace vostok {
namespace animation {

class animation_player;

namespace mixing {

class n_ary_tree_transition_tree_constructor;

class animated_object_holder {
public:
	inline	explicit	animated_object_holder	( pcvoid const animated_object ) :
		animated_object		( animated_object ),
		need_new_transform	( false )
	{
	}
	inline	bool		operator==				( pcvoid const animated_object ) const { return this->animated_object == animated_object; }

public:
	/* 0x0000 */	float4x4		transform;
	/* 0x0040 */	float4x4		new_transform;
	/* 0x0080 */	pcvoid const	animated_object;
	/* 0x0084 */	bool			need_new_transform;
}; // class animated_object_holder

STATIC_SIZE_ASSERT(animated_object_holder, 0x88);

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_MIXING_ANIMATED_OBJECT_HOLDER_H_INCLUDED
