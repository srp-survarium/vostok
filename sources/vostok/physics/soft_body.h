// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_PHYSICS_SOFT_BODY_H_INCLUDED
#define VOSTOK_PHYSICS_SOFT_BODY_H_INCLUDED

#include <BulletSoftBody/btSoftBody.h>
#include <vostok/physics/bullet_utils.h>

namespace vostok {
namespace physics {

class bullet_physics_world;

class bt_soft_body_rope {
	friend class bullet_physics_world;

public:
	explicit		bt_soft_body_rope	( btSoftBody* bt_body ) : m_bt_body( bt_body ) { }

	inline	bool	is_active			( ) const { return m_bt_body->isActive( ); }

	inline	u32		get_fragments_count	( ) const { return m_bt_body->m_links.size( ); }
	inline	void	get_fragment		( u32 idx, float3& pt0, float3& pt1 ) const
	{
		btSoftBody::Link const& link = m_bt_body->m_links[idx];
		pt0 = from_bullet( link.m_n[0]->m_x );
		pt1 = from_bullet( link.m_n[1]->m_x );
	}
	inline	u32		get_nodes_count		( ) const { return m_bt_body->m_nodes.size( ); }
	inline	void	get_node			( u32 idx, float3& pt ) const
	{
		btSoftBody::Node const& node = m_bt_body->m_nodes[idx];
		pt = from_bullet( node.m_x );
	}


private:
	/* 0x0000 */	btSoftBody*		m_bt_body;
}; // class bt_soft_body_rope

STATIC_SIZE_ASSERT(bt_soft_body_rope, 0x4);

//
// rope_construction_info
//

struct rope_construction_info  {
public:
	rope_construction_info() {}

public:
	/* 0x0000 */ float		kVCF;
	/* 0x0004 */ float		kDP;
	/* 0x0008 */ float		kDG;
	/* 0x000c */ float		kLF;
	/* 0x0010 */ float		kPR;
	/* 0x0014 */ float		kVC;
	/* 0x0018 */ float		kDF;
	/* 0x001c */ float		kMT;
	/* 0x0020 */ float		kCHR;
	/* 0x0024 */ float		kKHR;
	/* 0x0028 */ float		kSHR;
	/* 0x002c */ float		kAHR;
	/* 0x0030 */ float		timescale;
	/* 0x0034 */ s32		fragments_count;
	/* 0x0038 */ float		margin;
	/* 0x003c */ s32		iterations;
	/* 0x0040 */ float3		p0;
	/* 0x004c */ float3		p1;
	/* 0x0058 */ float		stiftness;
}; // struct rope_construction_info

STATIC_SIZE_ASSERT(rope_construction_info, 0x5C);

/*
VOSTOK_PHYSICS_API bt_soft_body_rope* create_soft_body_rope	( world* world, rope_construction_info const& construction_info );
VOSTOK_PHYSICS_API void				destroy_soft_body_rope	(  world* world, bt_soft_body_rope* body );
*/
} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_SOFT_BODY_H_INCLUDED
