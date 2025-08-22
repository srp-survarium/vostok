////////////////////////////////////////////////////////////////////////////
//	Created		: 10.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_VISUAL_H_INCLUDED
#define VOSTOK_RENDER_BASE_VISUAL_H_INCLUDED

namespace vostok {

namespace collision {
	struct geometry;
} // namespace collision

namespace render {

class visual : public resources::unmanaged_resource {
public:
	virtual	void				render			( )	= 0;
	virtual	void				render_selected	( ) = 0;
	virtual	collision::geometry const* get_collision_geometry	( )	= 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( visual )
}; // class visual

typedef	resources::resource_ptr<
			visual,
			resources::unmanaged_intrusive_base
		> visual_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_BASE_VISUAL_H_INCLUDED