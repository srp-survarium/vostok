#include "pch.h"

#include <vostok/console_command.h>

namespace vostok {
namespace render {

static bool s_dxt_env_probe = true;
static console_commands::cc_bool s_dxt_env_probe_cc(
	"r_dxt_env_probe",
	s_dxt_env_probe,
	false,
	console_commands::command_type_user_specific
);

} // namespace render
} // namespace vostok
