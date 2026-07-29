////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\ENGINE\SOURCES\SHADOW_CASCADE_VOLUME_H_INCLUDED
#define VOSTOK\RENDER\ENGINE\SOURCES\SHADOW_CASCADE_VOLUME_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline void shadow_cascade_volume::compute_planes( )
{
	// FUNCTION BODY[0x76a20]: 18
	// <0x76a2b>|0x00b|+0x196:'39'
	// <0x76bc1>|0x1a1|-0x18b:'39'
	// <0>
	// <1>
	// <2>
	// <0x76a36>|0x016|+0x191:'43'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// ******
}

// STATE[STUB]
inline void shadow_cascade_volume::compute_caster_model_fixed(
	fixed_vector< math::plane, 16 >&	dest,
	float3&								translation,
	float								map_size,
	bool								clip_by_view_near
)
{
	// LOCALS
	// s32[2] 							align_planes
	// float3 							align_vector
	// s32 								align_planes_count
	// s32 								p
	// float3 							tmp_point
	// float 							max_mag
	// float 							plane_dot_ray
	// u32 								i
	// float 							sign
	// float3 							tmp_vector
	// math::plane 						tmp_plane
	// math::plane 						plane
	// float3 							perp_light_to_view
	// float3 							origin
	// float 							dist
	// ******

	// FUNCTION BODY[0x76be0]: 183
	// <0x76bec>|0x00c|+0x011:'61'
	// <0>
	// <0x76bfd>|0x01d|+0x077:'63'
	// <0>
	// <1>
	// <2>
	// <0x76c74>|0x094|+0x06c:'67'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x76ce0>|0x100|-0x066:'78'
	// <0>
	// <0x76c7a>|0x09a|+0x048:'80'
	// <0x76cc2>|0x0e2|+0x014:'81'
	// <0>
	// <1>
	// <0x76cd6>|0x0f6|+0x004:'84'
	// <0x76cda>|0x0fa|+0x001:'85'
	// <0>
	// <0x76cdb>|0x0fb|+0x00e:'87'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x76ce9>|0x109|+0x003:'93'
	// <0>
	// <1>
	// <0x76cec>|0x10c|+0x2d2:'96'
	// <0x76fbe>|0x3de|-0x28e:'96'
	// <0>
	// <1>
	// <0x76d30>|0x150|+0x008:'99'
	// <0x76d38>|0x158|+0x183:'100'
	// <0x76ebb>|0x2db|-0x171:'100'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x76d4a>|0x16a|+0x182:'105'
	// <0x76ecc>|0x2ec|-0x100:'105'
	// <0x76dcc>|0x1ec|+0x171:'106'
	// <0x76f3d>|0x35d|+0x01c:'106'
	// <0>
	// <1>
	// <0x76f59>|0x379|+0x02e:'109'
	// <0x76f87>|0x3a7|+0x00c:'110'
	// <0x76f93>|0x3b3|+0x02e:'111'
	// <0>
	// <1>
	// <0x76fc1>|0x3e1|+0x029:'114'
	// <0>
	// <1>
	// <0x76fea>|0x40a|+0x018:'117'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x77002>|0x422|+0x05e:'127'
	// <0>
	// <0x77060>|0x480|+0x009:'129'
	// <0x77069>|0x489|+0x027:'130'
	// <0>
	// <0x77090>|0x4b0|+0x035:'132'
	// <0x770c5>|0x4e5|+0x00f:'133'
	// <0>
	// <1>
	// <0x770d4>|0x4f4|+0x053:'136'
	// <0>
	// <0x77127>|0x547|+0x039:'138'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x77160>|0x580|+0x00a:'143'
	// <0>
	// <0x7716a>|0x58a|+0x068:'145'
	// <0>
	// <1>
	// <2>
	// <0x771d2>|0x5f2|+0x027:'149'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x771f9>|0x619|+0x026:'155'
	// <0x7721f>|0x63f|+0x072:'156'
	// <0>
	// <1>
	// <0x77291>|0x6b1|+0x1a6:'159'
	// <0x77437>|0x857|-0x17d:'159'
	// <0x772ba>|0x6da|+0x034:'160'
	// <0x772ee>|0x70e|+0x123:'161'
	// <0>
	// <1>
	// <0x77411>|0x831|+0x21c:'164'
	// <0x7762d>|0xa4d|-0x1ed:'164'
	// <0>
	// <1>
	// <0x77440>|0x860|+0x07b:'167'
	// <0>
	// <1>
	// <0x774bb>|0x8db|+0x04b:'170'
	// <0>
	// <1>
	// <0x77506>|0x926|+0x020:'173'
	// <0>
	// <1>
	// <2>
	// <0x77526>|0x946|+0x08e:'177'
	// <0>
	// <0x775b4>|0x9d4|+0x00e:'179'
	// <0>
	// <0x775c2>|0x9e2|+0x094:'181'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x77656>|0xa76|+0x059:'186'
	// <0>
	// <1>
	// <0x776af>|0xacf|+0x058:'189'
	// <0x77707>|0xb27|+0x055:'190'
	// <0>
	// <0x7775c>|0xb7c|+0x046:'192'
	// <0x777a2>|0xbc2|+0x008:'193'
	// <0>
	// <1>
	// <0x777aa>|0xbca|+0x16c:'196'
	// <0x77916>|0xd36|-0x0c6:'196'
	// <0x77850>|0xc70|+0x0e0:'197'
	// <0x77930>|0xd50|+0x035:'197'
	// <0>
	// <0x77965>|0xd85|+0x014:'199'
	// <0>
	// <0x77979>|0xd99|+0x005:'201'
	// <0x7797e>|0xd9e|+0x04f:'202'
	// <0>
	// <0x779cd>|0xded|+0x227:'204'
	// <0>
	// <0x77bf4>|0x1014|-0x1f4:'206'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x77a00>|0xe20|+0x1f7:'211'
	// <0x77bf7>|0x1017|-0x1ee:'211'
	// <0>
	// <1>
	// <0x77a09>|0xe29|+0x02d:'214'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x77a36>|0xe56|+0x1ca:'220'
	// <0x77c00>|0x1020|-0x194:'220'
	// <0x77a6c>|0xe8c|+0x02a:'221'
	// <0x77a96>|0xeb6|+0x119:'222'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x77baf>|0xfcf|+0x057:'227'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x77c06>|0x1026|+0x0e9:'233'
	// <0x77cef>|0x110f|+0x0e8:'233'
	// <0x77dd7>|0x11f7|+0x0e9:'233'
	// <0x77ec0>|0x12e0|-0x27c:'233'
	// <0x77c44>|0x1064|+0x0e9:'234'
	// <0x77d2d>|0x114d|+0x0e8:'234'
	// <0x77e15>|0x1235|+0x0e9:'234'
	// <0x77efe>|0x131e|-0x2b5:'234'
	// <0x77c49>|0x1069|+0x0e9:'235'
	// <0x77d32>|0x1152|+0x0e8:'235'
	// <0x77e1a>|0x123a|+0x0e9:'235'
	// <0x77f03>|0x1323|-0x2b5:'235'
	// <0x77c4e>|0x106e|+0x0e9:'236'
	// <0x77d37>|0x1157|+0x0e8:'236'
	// <0x77e1f>|0x123f|+0x0e9:'236'
	// <0x77f08>|0x1328|-0x22a:'236'
	// <0>
	// <0x77cde>|0x10fe|+0x0e8:'238'
	// <0x77dc6>|0x11e6|+0x0e9:'238'
	// <0x77eaf>|0x12cf|+0x0e8:'238'
	// <0x77f97>|0x13b7|-0x2ab:'238'
	// <0x77cec>|0x110c|+0x0e8:'239'
	// <0x77dd4>|0x11f4|+0x0e9:'239'
	// <0x77ebd>|0x12dd|+0x0e8:'239'
	// <0x77fa5>|0x13c5|-0x3c1:'239'
	// <0>
	// <1>
	// <0x77be4>|0x1004|+0x3c4:'242'
	// <0x77fa8>|0x13c8|+0x069:'242'
	// <0>
	// ******
}

// STATE[STUB]
inline void* stage_shadow_direct::`scalar deleting destructor'( u32 arg_0 )
{
	return NULL;

	// FUNCTION BODY[0x781b0]: 29
	// <0>
	// <1>
	// <2>
	// <0x78023>|0x003|+0x157:'251'
	// <0x7817a>|0x15a|-0x0ea:'251'
	// <0>
	// <1>
	// <0x78090>|0x070|+0x008:'254'
	// <0x78098>|0x078|+0x041:'255'
	// <0x780d9>|0x0b9|+0x026:'256'
	// <0>
	// <0x780ff>|0x0df|+0x023:'258'
	// <0>
	// <1>
	// <0x78122>|0x102|+0x004:'261'
	// <0>
	// <0x78126>|0x106|+0x01f:'263'
	// <0x78145>|0x125|-0x008:'263'
	// <0x7813d>|0x11d|+0x016:'264'
	// <0x78153>|0x133|-0x012:'264'
	// <0x78141>|0x121|+0x016:'265'
	// <0x78157>|0x137|-0x014:'265'
	// <0x78143>|0x123|+0x016:'266'
	// <0x78159>|0x139|+0x002:'266'
	// <0>
	// <1>
	// <0x7815b>|0x13b|+0x02e:'269'
	// <0>
	// <1>
	// <0x78189>|0x169|+0x005:'272'
	// <0>
	// <1>
	// <0x7818e>|0x16e|+0x011:'275'
	// <0x7819f>|0x17f|+0x005:'276'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::geometry_batch*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_buffer_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::speedtree_forest::tree_render_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\ENGINE\SOURCES\SHADOW_CASCADE_VOLUME_H_INCLUDED
