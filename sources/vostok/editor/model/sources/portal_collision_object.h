////////////////////////////////////////////////////////////////////////////
//	Created		: 16.06.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef PORTAL_COLLISION_OBJECT_H_INCLUDED
#define PORTAL_COLLISION_OBJECT_H_INCLUDED

#include <vostok\collision\collision_object.h>
#include <vostok\collision\geometry.h>

namespace vostok {
namespace model_editor {
ref class edit_portal;
class portal_collision_object : public collision::collision_object
{
	typedef collision::collision_object	super;
public:
	portal_collision_object	( collision::geometry const* geometry, edit_portal^ portal );
	~portal_collision_object( );
	virtual bool	touch	( ) const;
private:
	gcroot<edit_portal^> m_portal;
}; // class portal_collision_object

} // namespace model_editor
} // namespace vostok

#endif // #ifndef PORTAL_COLLISION_OBJECT_H_INCLUDED

