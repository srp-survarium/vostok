////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_2_SOFT_BODY_H_INCLUDED
#define VOSTOK_PHYSICS_2_SOFT_BODY_H_INCLUDED

class btSoftBody;

namespace vostok {
namespace physics_2 {

// sushi@NOTE: OG soft_body has more info

class bt_soft_body_rope  {
public:
	bt_soft_body_rope			( btSoftBody* ) /* no source */;

	bool	is_active			( ) const /* no source */;

	u32		get_fragments_count	( ) const /* no source */;
	void	get_fragment		( u32, float3&, float3& ) const /* no source */;
	u32		get_nodes_count		( ) const /* no source */;
	void	get_node			( u32, float3& ) const /* no source */;


private:
	/* offset 0x0000 */ btSoftBody*                         m_bt_body;
}; // class bt_soft_body_rope

namespace {
	typedef char size_assert[
		sizeof(bt_soft_body_rope) == 0x4 ? 1 : -1
	];
}

//
// rope_construction_info
//

struct rope_construction_info  {
public:
	rope_construction_info() {}


private:
	/* offset 0x0000 */ float                               kVCF;
	/* offset 0x0004 */ float                               kDP;
	/* offset 0x0008 */ float                               kDG;
	/* offset 0x000c */ float                               kLF;
	/* offset 0x0010 */ float                               kPR;
	/* offset 0x0014 */ float                               kVC;
	/* offset 0x0018 */ float                               kDF;
	/* offset 0x001c */ float                               kMT;
	/* offset 0x0020 */ float                               kCHR;
	/* offset 0x0024 */ float                               kKHR;
	/* offset 0x0028 */ float                               kSHR;
	/* offset 0x002c */ float                               kAHR;
	/* offset 0x0030 */ float                               timescale;
	/* offset 0x0034 */ s32                                 fragments_count;
	/* offset 0x0038 */ float                               margin;
	/* offset 0x003c */ s32                                 iterations;
	/* offset 0x0040 */ float3                              p0;
	/* offset 0x004c */ float3                              p1;
	/* offset 0x0058 */ float                               stiftness;
}; // struct rope_construction_info

namespace {
	typedef char size_assert[
		sizeof(rope_construction_info) == 0x5C ? 1 : -1
	];
}

/*
VOSTOK_PHYSICS_API bt_soft_body_rope* create_soft_body_rope	( world* world, rope_construction_info const& construction_info );
VOSTOK_PHYSICS_API void				destroy_soft_body_rope	(  world* world, bt_soft_body_rope* body );
*/
} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_2_SOFT_BODY_H_INCLUDED