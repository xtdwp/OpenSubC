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
#include <Eigen/Eigen>


int main(int argc, char* argv[]) {
	//Eigen::MatrixXd dense(3, 3);
	//dense << 1, 0, 0,
	//	0, 1, 0,
	//	0, 0, 1;
	//Eigen::Vector3d v = { 1,2,3 };
	//Eigen::SparseMatrix<double, Eigen::ColMajor> sv(3, 2);
	//sv.insert(0, 0) = 1;
	//sv.insert(2, 0) = 3;
	//Eigen::SparseMatrix<double, Eigen::ColMajor> _sv(3, 2);
	//_sv.insert(1, 0) = 2;
	//Eigen::SparseMatrix<double, Eigen::ColMajor> x(3, 3);
	//std::vector<Eigen::Triplet<double>> values;
	//values.emplace_back(0, 0, 1);
	//values.emplace_back(2, 2, 2);
	//values.emplace_back(1, 1, 1);
	//x.setFromTriplets(values.begin(), values.end());
	////x = dense;
	//std::cout << "sv = \n" << sv << std::endl;
	//std::cout << "_sv = \n" << _sv << std::endl;
	//sv = sv + _sv;
	//std::cout << "sv = \n" << sv << std::endl;
	//std::cout << "x = \n" << x << std::endl;
	//sv.col(1) = x * sv.col(0);
	//std::cout << "sv = \n" << sv << std::endl;
	opensubc::initialize();
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