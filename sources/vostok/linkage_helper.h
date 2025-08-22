////////////////////////////////////////////////////////////////////////////
//	Created		: 22.06.2009
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LINKAGE_HELPER_H_INCLUDED
#define VOSTOK_LINKAGE_HELPER_H_INCLUDED

#define VOSTOK_DECLARE_LINKAGE_ID( linkage_id )  					\
		namespace vostok {											\
			namespace linkage_helpers {								\
				int linkage_id;										\
			}														\
		} // namespace vostok

#define VOSTOK_INCLUDE_TO_LINKAGE( linkage_id )  					\
		namespace vostok {											\
			namespace linkage_helpers {								\
				extern int linkage_id;								\
				static int* linkage_id ## refferer = &linkage_id;	\
			}														\
		} // namespace vostok

// #else // ##ifdef VOSTOK_STATIC_LIBRARIES
// #	define VOSTOK_DECLARE_LINKAGE_ID( linkage_id )
// #	define VOSTOK_INCLUDE_TO_LINKAGE( linkage_id )
// #endif // #ifndef VOSTOK_STATIC_LIBRARIES

#endif // #ifndef VOSTOK_LINKAGE_HELPER_H_INCLUDED
