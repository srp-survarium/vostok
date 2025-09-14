#ifndef VOSTOK_MACRO_TODO_H_INCLUDED
#define VOSTOK_MACRO_TODO_H_INCLUDED

#ifdef VOSTOK_TODO
#	error do not define VOSTOK_TODO macro
#endif // #ifdef VOSTOK_TODO

#define VOSTOK_TODO(message_to_show)	__FILE__ "(" VOSTOK_MAKE_STRING(__LINE__) "): @todo: " message_to_show

#if defined(GCC_VERSION) && ( GCC_VERSION < 4 )
	// to disable unknown pragma warning message
#	pragma warning( disable : ??? )
#endif // #if defined(GCC_VERSION) && ( GCC_VERSION < 4.5 )

#endif // #ifndef VOSTOK_MACRO_TODO_H_INCLUDED