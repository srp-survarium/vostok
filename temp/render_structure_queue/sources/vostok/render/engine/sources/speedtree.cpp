////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\speedtree.h"

namespace vostok {
namespace render {

// STATE[STUB]
void* speed_tree_allocator::Alloc( u32 size )
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5348c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN11") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x53486 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN12") })
	// ******

	return NULL;

	// FUNCTION BODY[0x64440]: 8
	// <0x64440>|0x000|+0x014:'55'
	// <0>
	// <0x64454>|0x014|+0x007:'57'
	// <0>
	// <0x6445b>|0x01b|+0x004:'59'
	// <0x6445f>|0x01f|+0x027:'60'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void speed_tree_allocator::Free( void* block )
{
	// FUNCTION BODY[0x64490]: 2
	// <0x64490>|0x000|+0x008:'66'
	// <0x64498>|0x008|+0x014:'67'
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_speedtree_cook''( )
{
	// FUNCTION BODY[0x7d5a80]
	// ******
}

// STATE[STUB]
void `vostok::render::initialize_speedtree'::`2'::`dynamic atexit destructor for 's_speed_tree_allocator''( )
{
	// FUNCTION BODY[0x7edb30]
	// ******
}

// STATE[STUB]
void initialize_speedtree( )
{
	// LOCALS
	// SpeedTree::CAllocatorInterface 	cOn
	// ******

	// STATICS
	// static speed_tree_allocator 		s_speed_tree_allocator = <0x4c2689c>;
	// ******

	// FUNCTION BODY[0x76a880]: 16
	// <0x76a882>|0x002|+0x02b:'76'
	// <0>
	// <0x76a8ad>|0x02d|+0x005:'78'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x76a8b2>|0x032|+0x00e:'83'
	// <0>
	// <0x76a8c0>|0x040|+0x00c:'85'
	// <0>
	// <0x76a8cc>|0x04c|+0x013:'87'
	// <0x76a8df>|0x05f|+0x00b:'88'
	// <0>
	// <0x76a8ea>|0x06a|+0x011:'90'
	// <0x76a8fb>|0x07b|+0x00d:'91'
	// ******
}

// STATE[STUB]
void finalize_speedtree( )
{
	// CALL SITE INFO
	// <0x76aa28> -> void* < unknown >( u32 )
	// <0x76aa4b> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x76aa10]: 5
	// <0x76aa11>|0x001|+0x007:'96'
	// <0x76aa18>|0x008|+0x01c:'97'
	// <0>
	// <0x76aa34>|0x024|+0x007:'99'
	// <0x76aa3b>|0x02b|+0x01d:'100'
	// ******
}

// STATE[STUB]
void print_speedtree_errors( )
{
	// FUNCTION BODY[0x76a910]: 6
	// <0>
	// <0x76a919>|0x009|+0x017:'106'
	// <0>
	// <0x76a930>|0x020|+0x0ba:'108'
	// <0x76a9ea>|0x0da|+0x00f:'109'
	// <0>
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
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
