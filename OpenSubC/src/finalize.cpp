#include "finalize.h"
#include "capi.h"
#include "settings.h"

namespace opensubc {

    void free_memory()
    {
        //ÊÍ·ÅÄÚ´æ
        //free_memory_geometry();
        free_memory_settings();
        
    }

}

using namespace opensubc;

int opensubc_finalize()
{
    // Clear results
    //opensubc_reset();

    // Reset timers
    //reset_timers();

    // Reset global variables
    //settings::assume_separate = false;
  

    // Deallocate arrays
    free_memory();

    // Free all MPI types
#ifdef OPENSUBC_MPI
    if (mpi::bank != MPI_DATATYPE_NULL) MPI_Type_free(&mpi::bank);
#endif

    return 0;
}

int opensubc_reset()
{
    return 0;
}

int opensubc_hard_reset()
{
    return 0;
}