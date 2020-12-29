#ifndef OPENSUBC_SETTINGS_H
#define OPENSUBC_SETTINGS_H


#include "constants.h"

namespace opensubc {

	//==============================================================================
	// Global variable declarations
	//==============================================================================

	namespace settings {

		extern int run_mode;
		extern int verbosity;

	} // namespace settings

	//==============================================================================
	// Functions
	//==============================================================================

	//! Read settings from XML file
	//! \param[in] root XML node for <settings>
	void read_settings_xml();

	void free_memory_settings();

} // namespace opensubc

#endif // OPENSUBC_SETTINGS_H