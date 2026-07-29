////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_NONCOPYABLE_PROTECTED_H_INCLUDED
#define RENDER_NONCOPYABLE_PROTECTED_H_INCLUDED

namespace vostok {
namespace render {

class noncopyable_protected {
protected:
	inline		noncopyable_protected	( ) { /* no source */ }
	inline		~noncopyable_protected	( ) { /* no source */ }
}; // class noncopyable_protected

STATIC_SIZE_ASSERT(noncopyable_protected, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_NONCOPYABLE_PROTECTED_H_INCLUDED
