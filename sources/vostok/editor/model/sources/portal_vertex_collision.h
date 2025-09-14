////////////////////////////////////////////////////////////////////////////
//	Created		: 10.06.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef PORTAL_VERTEX_COLLISION_H_INCLUDED
#define PORTAL_VERTEX_COLLISION_H_INCLUDED
#include <vostok/collision/collision_object.h>

namespace vostok {
namespace model_editor {
ref class edit_portal;
class portal_vertex_collision : public collision::collision_object
{
	typedef collision::collision_object	super;
public:
	portal_vertex_collision	( collision::geometry const* geometry, edit_portal^ portal, int index );
	int				get_index				( ) const;
	void			set_index				( int index );
	virtual bool	touch					( ) const;
private:
	gcroot<edit_portal^> m_portal;
	int m_index;
}; // class portal_vertex_collision

} // namespace model_editor
} // namespace vostok

#endif // #ifndef PORTAL_VERTEX_COLLISION_H_INCLUDED