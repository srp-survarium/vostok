///////////////////////////////////////////////////////////////////////////
//	Created 	: 05.02.2008
//	Author		: Konstantin Slipchenko
//	Description : contact info
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_COLLISION_CONTACT_INFO_H_INCLUDED
#define VOSTOK_COLLISION_CONTACT_INFO_H_INCLUDED

namespace vostok {
namespace render {
namespace debug {
	class renderer;
} // namespace debug
} // namespace render

namespace collision {

struct contact_info
{
	float3	position;
	float3	normal;
	float	depth;
	u32		material_id;
}; // struct contact_info

struct VOSTOK_NOVTABLE on_contact
{
	virtual void						operator( ) ( const contact_info& c )	= 0;
	virtual void						change_order()							= 0;
	virtual	memory::base_allocator*		alocator( )								= 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( on_contact )
}; // struct on_contact

} // namespace collision
} // namespace vostok

#endif // #ifndef VOSTOK_COLLISION_CONTACT_INFO_H_INCLUDED