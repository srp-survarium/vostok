////////////////////////////////////////////////////////////////////////////
//	Created		: 14.10.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef NONCOPYABLE_H_INCLUDED
#define NONCOPYABLE_H_INCLUDED

#include <vostok/animation/api.h>

namespace vostok {
namespace animation {

class VOSTOK_ANIMATION_API noncopyable {
protected:
  inline noncopyable() {}
  inline ~noncopyable() {}

private:
  inline noncopyable( const noncopyable& );
  inline const noncopyable& operator=( const noncopyable& );
}; // class noncopyable

} // namespace animation
} // namespace vostok

#endif // #ifndef NONCOPYABLE_H_INCLUDED