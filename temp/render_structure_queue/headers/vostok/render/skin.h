////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKIN_H_INCLUDED
#define RENDER_SKIN_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

class skin : public resources::unmanaged_resource {
protected:
	virtual		~skin	( ) = 0;

public:
	inline		skin	( ) { /* no source */ }
}; // class skin

STATIC_SIZE_ASSERT(skin, 0x108);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKIN_H_INCLUDED
