#ifndef VOSTOK_RENDER_CORE_QUASI_SINGLETON_H_INCLUDED
#define VOSTOK_RENDER_CORE_QUASI_SINGLETON_H_INCLUDED

#include <vostok/debug/static_cast_checked.h>

namespace vostok {

template < typename T >
class quasi_singleton : private boost::noncopyable {
public:
	quasi_singleton( );
	~quasi_singleton( );

	static T* ptr( ) { return pinst; }
	static T& ref( );

private:
	static T* pinst;
};

template < typename T >
quasi_singleton< T >::quasi_singleton( )
{
	ASSERT( !pinst );
	pinst = static_cast_checked< T* >( this );
}

template < typename T >
quasi_singleton< T >::~quasi_singleton( )
{
	ASSERT( pinst );
	pinst = 0;
}

template < typename T >
T* quasi_singleton< T >::pinst = 0;

template < typename T >
T& quasi_singleton< T >::ref( )
{
	ASSERT( pinst );
	return *pinst;
}

} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_QUASI_SINGLETON_H_INCLUDED
