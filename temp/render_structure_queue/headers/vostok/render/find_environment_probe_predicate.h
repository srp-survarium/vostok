////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FIND_ENVIRONMENT_PROBE_PREDICATE_H_INCLUDED
#define RENDER_FIND_ENVIRONMENT_PROBE_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::environment_probe const* const;

namespace vostok {
namespace render {

struct find_environment_probe_predicate {
	inline	explicit	find_environment_probe_predicate( const u32 arg_0 ) { /* no source */ }

	inline	bool		operator()						( environment_probe const* const arg_0 ) { /* no source */ }

	/* 0x0000 */	u32		m_id;
}; // struct find_environment_probe_predicate

STATIC_SIZE_ASSERT(find_environment_probe_predicate, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FIND_ENVIRONMENT_PROBE_PREDICATE_H_INCLUDED
