////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SCALEFORM_LOG_H_INCLUDED
#define VOSTOK_SCALEFORM_LOG_H_INCLUDED

#include "Kernel/SF_Log.h"	// Scaleform::Log (base)

namespace survarium {

class vostok_scaleform_log : public Scaleform::Log {
private:
	// PDB shows the va_list argument as char* (its x86 MSVC definition);
	// out-of-line in factory.cpp per the rich index
	virtual	void	LogMessageVarg			( Scaleform::LogMessageId arg_0, pcstr arg_1, va_list arg_2 ) override;

public:
	inline			vostok_scaleform_log	( ) { /* no source */ }
	virtual			~vostok_scaleform_log	( ) { /* no source */ }
}; // class vostok_scaleform_log

STATIC_SIZE_ASSERT(vostok_scaleform_log, 0x8);

} // namespace survarium

#endif // #ifndef VOSTOK_SCALEFORM_LOG_H_INCLUDED
