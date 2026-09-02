// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_SCALEFORM_LOG_H_INCLUDED
#define VOSTOK_SCALEFORM_LOG_H_INCLUDED

#include "Kernel/SF_Log.h"	// Scaleform::Log (base)

namespace survarium {

class vostok_scaleform_log : public Scaleform::Log {
private:
	// PDB shows the va_list argument as char* (its x86 MSVC definition).
	virtual	void	LogMessageVarg			( Scaleform::LogMessageId arg_0, pcstr arg_1, va_list arg_2 ) override;

public:
}; // class vostok_scaleform_log

STATIC_SIZE_ASSERT(vostok_scaleform_log, 0x8);

} // namespace survarium

#endif // #ifndef VOSTOK_SCALEFORM_LOG_H_INCLUDED
