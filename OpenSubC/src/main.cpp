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
#include <fuelRod.h>
#include <channel.h>
#include <gap.h>




int main(int argc, char* argv[]) {
	/*Eigen::SparseMatrix<double> M;
	M.resize(2, 2);
	M.insert(0, 0) = 1;
	M.insert(0, 1) = 2;
	M.insert(1, 0) = 3;
	M.insert(1, 1) = 4;

	Eigen::Vector2d x, y;
	y[0] = 3;
	y[1] = 7;

	Eigen::SparseLU<Eigen::SparseMatrix<double>> sc;
	sc.compute(M);
	x = sc.solve(y);

	std::cout << M << std::endl << y << std::endl << x << std::endl;

	system("pause");*/

	opensubc::initialize();

    for (auto& rod : opensubc::geometry::rods)
	    std::cout << rod.toString() << std::endl;
	for (auto& gap : opensubc::geometry::gaps)
		std::cout << gap.toString() << std::endl;
	for (auto& channel : opensubc::geometry::channels)
		std::cout << channel.toString() << std::endl;
	opensubc::calculate();

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