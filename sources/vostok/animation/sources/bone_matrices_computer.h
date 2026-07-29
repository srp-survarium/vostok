////////////////////////////////////////////////////////////////////////////
//	Created		: 12.08.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef BONE_MATRICES_COMPUTER_H_INCLUDED
#define BONE_MATRICES_COMPUTER_H_INCLUDED

namespace vostok {
namespace animation {

namespace mixing {
	struct animation_state;
	class n_ary_tree;
} // namespace mixing

class skeleton;
class skeleton_bone;
struct bone_transform;
enum animation_types_enum;
class bone_animation;

class bone_matrices_computer : private boost::noncopyable {
public:
								bone_matrices_computer			(
									pcvoid const				animated_object,
									skeleton const*				skeleton,
									mixing::animation_state*	animations,
									const u32					animations_count
								);
								~bone_matrices_computer			( );

			void				compute_bones_matrices			( float4x4* begin, float4x4* end, u32 const* bones_masks ) const;
			void				compute_bones_local_matrices	( float4x4* begin, float4x4* end, u32 const* bones_masks ) const;
			void				convert_to_object_matrices		( float4x4* begin, float4x4* end ) const;
#ifndef MASTER_GOLD
			float4x4			computed_bone_matrix			( skeleton_bone const& bone ) const;
#endif // #ifndef MASTER_GOLD

private:
			float4x4			get_object_transform			( ) const;
	inline	bool				overweighting_detected			( ) const { /* no source */ }

private:
			bone_transform		computed_local_bone_transform	( skeleton_bone const& bone, u32 bone_mask, u32 animation_layer_id ) const;
			float4x4			computed_local_bone_matrix		( skeleton_bone const& bone, u32 bone_mask ) const;

			void				compute_skeleton_branch			(
									skeleton_bone const&	bone,
									float4x4*				result,
									float4x4 const&			parent,
									u32 const*				bone_mask,
									u32 const*				result_masks
								) const;
			void				compute_skeleton_branch_local	(
									skeleton_bone const&	bone,
									float4x4*				result,
									u32 const*				bone_mask,
									u32 const*				result_masks
								) const;

			void				convert_skeleton_branch			( skeleton_bone const& bone, float4x4* result, float4x4 const& parent ) const;
private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	pcvoid const						m_animated_object;
	/* 0x0004 */	skeleton const*						m_skeleton;
	/* 0x0008 */	mixing::animation_state* const		m_animations;
	/* 0x000c */	const u32							m_animations_count;
	/* 0x0010 */	u32									m_layers_count;
	/* 0x0014 */	mutable bool						m_overweighting_detected;

	friend class mixing::n_ary_tree;
}; // class bone_matrices_computer

STATIC_SIZE_ASSERT(bone_matrices_computer, 0x18);

} // namespace animation
} // namespace vostok

#endif // #ifndef BONE_MATRICES_COMPUTER_H_INCLUDED
