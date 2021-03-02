//#include <iostream>
//#include <Eigen/Dense>

//#include "opensubc/capi.h"
//#include "opensubc/constants.h"
//#include "opensubc/error.h"
//#include "opensubc/message_passing.h"
//#include "opensubc/particle_restart.h"
//#include "opensubc/settings.h"

#include "capi.h"
#include "settings.h"
#include "error.h"
#include "constants.h"
#include "geometry.h"
#include "calculations.h"
#include "initialize.h"
#include <iostream>


int main(int argc, char* argv[]) {
	if (!opensubc::inp.LoadFile("input.xml"))
	{
		std::cout << "fail" << std::endl;
	}
	opensubc::initialize_geometry();

//    using namespace opensubc;
//    int err;
//
//    // Initialize run -- when run with MPI, pass communicator
//#ifdef OPENSUBC_MPI
//    MPI_Comm world{ MPI_COMM_WORLD };
//    err = opensubc_init(argc, argv, &world);
//#else
//    err = opensubc_init(argc, argv, nullptr);
//#endif
//    if (err == -1) {
//        // This happens for the -h and -v flags
//        return 0;
//    }
//    else if (err) {
//        fatal_error(opensubc_err_msg);
//    }
//
//    // start problem based on mode
//    switch (settings::run_mode) {
//    case RUN_MODE_CALCULATING:
//        err = opensubc_run();
//        break;
//    case RUN_MODE_PLOTTING:
//        //err = opensubc_plot_geometry();
//        break;
//    }
//    if (err) fatal_error(opensubc_err_msg);
//
//    // Finalize and free up memory
//    err = opensubc_finalize();
//    if (err) fatal_error(opensubc_err_msg);

}