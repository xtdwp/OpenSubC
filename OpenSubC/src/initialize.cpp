#include "initialize.h"

#include <cstddef>
#include <cstdlib> // for getenv
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "capi.h"
#include "error.h"
#include "settings.h"
#include "constants.h"
#include "output.h"
#include "geometry.h"
#include "calculations.h"

TiXmlDocument opensubc::inp;

int opensubc_init(int argc, char* argv[], const void* intracomm)
{
    using namespace opensubc;

#ifdef OPENSUBC_MPI
    // Check if intracomm was passed
    MPI_Comm comm;
    if (intracomm) {
        comm = *static_cast<const MPI_Comm*>(intracomm);
    }
    else {
        comm = MPI_COMM_WORLD;
    }

    // Initialize MPI for C++
    initialize_mpi(comm);
#endif

    // Parse command-line arguments
    int err = parse_command_line(argc, argv);
    if (err) return err;

    // Start total and initialization timer
    /*simulation::time_total.start();
    simulation::time_initialize.start();*/

#ifdef _OPENMP
    // If OMP_SCHEDULE is not set, default to a static schedule
    char* envvar = std::getenv("OMP_SCHEDULE");
    if (!envvar) {
        omp_set_schedule(omp_sched_static, 0);
    }
#endif


    // Read XML input files
    /*read_input_xml();
    initialize_geometry();*/
    // Stop initialization timer
    /*simulation::time_initialize.stop();*/

    return 0;
}

namespace opensubc {

#ifdef OPENSUBC_MPI
    void initialize_mpi(MPI_Comm intracomm)
    {
        mpi::intracomm = intracomm;

        // Initialize MPI
        int flag;
        MPI_Initialized(&flag);
        if (!flag) MPI_Init(nullptr, nullptr);

        // Determine number of processes and rank for each
        MPI_Comm_size(intracomm, &mpi::n_procs);
        MPI_Comm_rank(intracomm, &mpi::rank);
        mpi::master = (mpi::rank == 0);

        // Create bank datatype
        Particle::Bank b;
        MPI_Aint disp[6];
        MPI_Get_address(&b.r, &disp[0]);
        MPI_Get_address(&b.u, &disp[1]);
        MPI_Get_address(&b.E, &disp[2]);
        MPI_Get_address(&b.wgt, &disp[3]);
        MPI_Get_address(&b.delayed_group, &disp[4]);
        MPI_Get_address(&b.particle, &disp[5]);
        for (int i = 5; i >= 0; --i) disp[i] -= disp[0];

        int blocks[]{ 3, 3, 1, 1, 1, 1 };
        MPI_Datatype types[]{ MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_INT, MPI_INT };
        MPI_Type_create_struct(6, blocks, disp, types, &mpi::bank);
        MPI_Type_commit(&mpi::bank);
    }
#endif // OPENSUBC_MPI


    int parse_command_line(int argc, char* argv[])
    {
//        int last_flag = 0;
//        for (int i = 1; i < argc; ++i) {
//            std::string arg{ argv[i] };
//            if (arg[0] == '-') {
//                if (arg == "-p" || arg == "--plot") {
//                    settings::run_mode = RUN_MODE_PLOTTING;
//                    settings::check_overlaps = true;
//
//                }
//                else if (arg == "-n" || arg == "--particles") {
//                    i += 1;
//                    settings::n_particles = std::stoll(argv[i]);
//
//                }
//                else if (arg == "-r" || arg == "--restart") {
//                    i += 1;
//
//                    // Check what type of file this is
//                    hid_t file_id = file_open(argv[i], 'r', true);
//                    std::string filetype;
//                    read_attribute(file_id, "filetype", filetype);
//                    file_close(file_id);
//
//                    // Set path and flag for type of run
//                    if (filetype == "statepoint") {
//                        settings::path_statepoint = argv[i];
//                        settings::restart_run = true;
//                    }
//                    else if (filetype == "particle restart") {
//                        settings::path_particle_restart = argv[i];
//                        settings::particle_restart_run = true;
//                    }
//                    else {
//                        std::stringstream msg;
//                        msg << "Unrecognized file after restart flag: " << filetype << ".";
//                        strcpy(opensubc_err_msg, msg.str().c_str());
//                        return OPENSUBC_E_INVALID_ARGUMENT;
//                    }
//
//                    // If its a restart run check for additional source file
//                    if (settings::restart_run && i + 1 < argc) {
//                        // Check if it has extension we can read
//                        if (ends_with(argv[i + 1], ".h5")) {
//
//                            // Check file type is a source file
//                            file_id = file_open(argv[i + 1], 'r', true);
//                            read_attribute(file_id, "filetype", filetype);
//                            file_close(file_id);
//                            if (filetype != "source") {
//                                std::string msg{ "Second file after restart flag must be a source file" };
//                                strcpy(opensubc_err_msg, msg.c_str());
//                                return OPENSUBC_E_INVALID_ARGUMENT;
//                            }
//
//                            // It is a source file
//                            settings::path_sourcepoint = argv[i + 1];
//                            i += 1;
//
//                        }
//                        else {
//                            // Source is in statepoint file
//                            settings::path_sourcepoint = settings::path_statepoint;
//                        }
//
//                    }
//                    else {
//                        // Source is assumed to be in statepoint file
//                        settings::path_sourcepoint = settings::path_statepoint;
//                    }
//
//                }
//                else if (arg == "-g" || arg == "--geometry-debug") {
//                    settings::check_overlaps = true;
//                }
//                else if (arg == "-c" || arg == "--volume") {
//                    settings::run_mode = RUN_MODE_VOLUME;
//                }
//                else if (arg == "-s" || arg == "--threads") {
//                    // Read number of threads
//                    i += 1;
//
//#ifdef _OPENMP
//                    // Read and set number of OpenMP threads
//                    int n_threads = std::stoi(argv[i]);
//                    if (n_threads < 1) {
//                        std::string msg{ "Number of threads must be positive." };
//                        strcpy(opensubc_err_msg, msg.c_str());
//                        return OPENSUBC_E_INVALID_ARGUMENT;
//                    }
//                    omp_set_num_threads(n_threads);
//#else
//                    if (mpi::master)
//                        warning("Ignoring number of threads specified on command line.");
//#endif
//
//                }
//                else if (arg == "-?" || arg == "-h" || arg == "--help") {
//                    print_usage();
//                    return OPENSUBC_E_UNASSIGNED;
//
//                }
//                else if (arg == "-v" || arg == "--version") {
//                    print_version();
//                    return OPENSUBC_E_UNASSIGNED;
//
//                }
//                else if (arg == "-t" || arg == "--track") {
//                    settings::write_all_tracks = true;
//
//                }
//                else {
//                    std::cerr << "Unknown option: " << argv[i] << '\n';
//                    print_usage();
//                    return OPENSUBC_E_UNASSIGNED;
//                }
//
//                last_flag = i;
//            }
//        }
//
//        // Determine directory where XML input files are
//        if (argc > 1 && last_flag < argc - 1) {
//            settings::path_input = std::string(argv[last_flag + 1]);
//
//            // Add slash at end of directory if it isn't there
//            if (!ends_with(settings::path_input, "/")) {
//                settings::path_input += "/";
//            }
//        }

        return 0;
    }

    void initialize()
    {
        read_input_xml();
        initialize_geometry();
        std::cout << "geometry initialize successfully!" << std::endl;
        initialize_calculation();
        std::cout << "calculation initialize successfully!" << std::endl;
    }

    void read_input_xml()
    {
        if (!opensubc::inp.LoadFile("input.xml"))
        {
            std::cout << "fail to read input.xml" << std::endl;
        }
        //read_settings_xml();
        //read_cross_sections_xml();
        //read_materials_xml();
        //read_geometry_xml();


        //if (settings::run_mode != RUN_MODE_PLOTTING) {
        //    simulation::time_read_xs.start();
        //    if (settings::run_CE) {
        //        // Read continuous-energy cross sections
        //        read_ce_cross_sections(nuc_temps, thermal_temps);
        //    }
        //    else {
        //        // Create material macroscopic data for MGXS
        //        read_mgxs();
        //        create_macro_xs();
        //    }
        //    simulation::time_read_xs.stop();
        //}

        //read_tallies_xml();

        //// Initialize distribcell_filters
        //prepare_distribcell();

        //if (settings::run_mode == RUN_MODE_PLOTTING) {
        //    // Read plots.xml if it exists
        //    read_plots_xml();
        //    if (mpi::master && settings::verbosity >= 5) print_plot();

        //}
        //else {
        //    // Write summary information
        //    if (mpi::master && settings::output_summary) write_summary();

        //    // Warn if overlap checking is on
        //    if (mpi::master && settings::check_overlaps) {
        //        warning("Cell overlap checking is ON.");
        //    }
        //}

    }

} // namespace opensubc