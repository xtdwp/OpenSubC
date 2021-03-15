#include "settings.h"

namespace opensubc {

//==============================================================================
// Global variables
//==============================================================================

namespace settings {

	int run_mode{ -1 }; 
	int verbosity{ 7 };

	// 全局变量的初始化都放在这里，例如：
	//bool assume_separate         {false};
	//std::string path_input;
	//std::string path_output;


} // namespace settings

//==============================================================================
// Functions
//==============================================================================


void read_settings_xml()
{
	using namespace settings;

}

void free_memory_settings() {

}


}// namespace opensubc