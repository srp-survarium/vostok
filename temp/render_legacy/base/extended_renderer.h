////////////////////////////////////////////////////////////////////////////
//	Created 	: 18.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_EXTENDED_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_BASE_EXTENDED_RENDERER_H_INCLUDED

namespace vostok {
namespace render {
namespace extended {

struct VOSTOK_NOVTABLE renderer {
	virtual	pcstr	type		( ) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( renderer )
}; // class world

} // namespace extended
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_BASE_EXTENDED_RENDERER_H_INCLUDED