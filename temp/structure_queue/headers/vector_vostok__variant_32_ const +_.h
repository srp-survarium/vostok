////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::variant<32> const *,survarium::std_allocator<vostok::variant<32> const *> >
	stlp_std::vector<vostok::variant<32> const * >;
class vostok::variant<32> const *;

/* FORWARD REFS */
class vostok::variant<32>;
class vostok::variant<32> const*;

namespace survarium {

class vector< variant< 32 > const* > : public std::vector< variant< 32 > const* > {
public:
	inline									vector<vostok::variant<32> const *>	( ) { /* no source */ }
	inline									vector<vostok::variant<32> const *>	( u32 arg_0, variant< 32 > const* const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::variant<32> const *>	( u32 arg_0 ) { /* no source */ }

	inline	variant< 32 > const*&			operator[]							( u32 arg_0 ) { /* no source */ }
	inline	variant< 32 > const* const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32								size								( ) const { /* no source */ }

	inline									~vector<vostok::variant<32> const *>( ) { /* no source */ }
}; // class vector< variant< 32 > const* >

STATIC_SIZE_ASSERT(vector< variant< 32 > const* >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
