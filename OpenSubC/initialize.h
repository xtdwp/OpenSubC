#ifndef OPENSUBC_INITIALIZE_H
#define OPENSUBC_INITIALIZE_H

#ifdef OPENSUBC_MPI
#include "mpi.h"
#endif

namespace opensubc {

	int parse_command_line(int argc, char* argv[]);
#ifdef OPENSUBC_MPI
	void initialize_mpi(MPI_Comm intracomm);
#endif
	void read_input_xml();

}

#endif // OPENSUBC_INITIALIZE_H