////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_STR_PRED_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_STR_PRED_H_INCLUDED

/* INCLUDES */
struct stlp_std::binary_function<char *,char *,bool>;
class vostok::fs_new::virtual_path_string;

namespace vostok {
namespace render {

struct resource_manager::str_pred : public std::binary_function< char*, char*, bool > {
	inline	bool	operator()	( fs_new::virtual_path_string arg_0, fs_new::virtual_path_string arg_1 ) const { /* no source */ }

	inline			~str_pred	( ) { /* no source */ }
}; // struct resource_manager::str_pred

STATIC_SIZE_ASSERT(resource_manager::str_pred, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_STR_PRED_H_INCLUDED
