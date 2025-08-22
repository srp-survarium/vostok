#ifndef VOSTOK_UNREFERENCED_PARAMETER_H_INCLUDED
#define VOSTOK_UNREFERENCED_PARAMETER_H_INCLUDED


namespace vostok {
namespace detail {
	inline void	unreferenced_parameter_helper ( ... ) {}
} // namespace detail
} // namespace vostok

#define VOSTOK_UNREFERENCED_PARAMETER(parameter) \
			(void)(&parameter)

#define VOSTOK_UNREFERENCED_PARAMETERS(...) \
	if ( vostok::identity(false) ) { vostok::detail::unreferenced_parameter_helper(__VA_ARGS__); } else (void)0

#endif // #ifndef VOSTOK_UNREFERENCED_PARAMETER_H_INCLUDED