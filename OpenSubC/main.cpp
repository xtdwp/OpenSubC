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
	for (auto &rod:opensubc::geometry::rods)
	{
		std::cout << rod.id << " " << rod.x << " " << rod.y << " " << rod.r << std::endl;
		for (auto& id : rod.channelIds)
			std::cout << id << " ";
		std::cout << std::endl;
		for (auto& id : rod.gapIds)
			std::cout << id << " ";
		std::cout << std::endl;
	}
	for (auto& gap : opensubc::geometry::gaps)
	{
		std::cout << gap.id << std::endl;
		std::cout << gap.rodId[0] << " " << gap.rodId[1] << std::endl;
		std::cout << gap.channelId[0] << " " << gap.channelId[1] << std::endl;
	}
	for (auto& channel : opensubc::geometry::channels)
	{
		std::cout << channel.id << " " << channel.A << std::endl;
		for (auto& id : channel.rodIds)
			std::cout << id << " ";
		std::cout << std::endl;
		for (auto& id : channel.gapIds)
			std::cout << id << " ";
		std::cout << std::endl;
		for (auto& id : channel.circleLength)
			std::cout << id << " ";
		std::cout << std::endl;
	}

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