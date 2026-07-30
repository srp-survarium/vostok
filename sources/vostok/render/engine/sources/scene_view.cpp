#include "pch.h"
#include "scene_view.h"

#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_movie_resource.h>

namespace vostok {
namespace render {

struct priority_less : public std::binary_function<
	survarium::flash_movie_resource_ptr const&,
	survarium::flash_movie_resource_ptr const&,
	bool
> {
	bool operator()(
		survarium::flash_movie_resource_ptr const& left,
		survarium::flash_movie_resource_ptr const& right
	) const
	{
		return left->movie->GetPriority( ) < right->movie->GetPriority( );
	}
};

STATIC_SIZE_ASSERT( priority_less, 0x1 );

// STATE[STUB]
 scene_view::scene_view( )
{
	// FUNCTION BODY[0x638550]: 30
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
	// <13>
	// <14>
	// <15>
	// <16>
	// <0x63863d>|0x0ed|+0x0e4:'50'
	// <0x638721>|0x1d1|+0x01e:'51'
	// <0x63873f>|0x1ef|+0x028:'52'
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
	// ******
}

// STATE[STUB]
 scene_view::~scene_view( )
{
	// FUNCTION BODY[0x6383b0]: 23
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
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// ******
}

// STATE[STUB]
void scene_view::add_movie( survarium::flash_movie_resource_ptr& movie )
{
	// FUNCTION BODY[0x638350]: 4
	// <0>
	// <1>
	// <0x638354>|0x004|+0x043:'120'
	// <0x638397>|0x047|+0x015:'121'
	// ******
}

// STATE[STUB]
void scene_view::remove_movie( survarium::flash_movie_resource_ptr& movie )
{
	// FUNCTION BODY[0x638320]: 3
	// <0x638322>|0x002|+0x016:'126'
	// <0>
	// <0x638338>|0x018|+0x009:'128'
	// ******
}

// STATE[STUB]
void scene_view::add_text_manager( survarium::flash_text_manager* tm )
{
	// FUNCTION BODY[0x638310]: 5
	// <0>
	// <0x638310>|0x000|+0x006:'134'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void scene_view::remove_text_manager( survarium::flash_text_manager* tm )
{
	// FUNCTION BODY[0x638300]: 5
	// <0>
	// <0x638300>|0x000|+0x00a:'143'
	// <0>
	// <1>
	// <2>
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
	// 	survarium::flash_movie_resource_ptr*
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
	// 	vostok::render::light_ptr*
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
