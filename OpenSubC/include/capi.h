#ifndef OPENSUBC_CAPI_H
#define OPENSUBC_CAPI_H
//capi.h�ﶨ�����й��û�ʹ�õĽӿں�����
//���ǲ�û��capi.cpp�����ﺯ���ĸ���ʵ��д�ڶ�Ӧģ���cpp��,�����cpp�������capi.h

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    int opensubc_init(int argc, char* argv[], const void* intracomm);
    int opensubc_run();
    int opensubc_finalize();
    int opensubc_reset();
    int opensubc_hard_reset();

    // Error codes
    extern int OPENSUBC_E_UNASSIGNED;
    extern int OPENSUBC_E_ALLOCATE;
    extern int OPENSUBC_E_OUT_OF_BOUNDS;
    extern int OPENSUBC_E_INVALID_SIZE;
    extern int OPENSUBC_E_INVALID_ARGUMENT;
    extern int OPENSUBC_E_INVALID_TYPE;
    extern int OPENSUBC_E_INVALID_ID;
    extern int OPENSUBC_E_GEOMETRY;
    extern int OPENSUBC_E_DATA;
    extern int OPENSUBC_E_PHYSICS;
    extern int OPENSUBC_E_WARNING;

    // Global variables
    extern char opensubc_err_msg[256];

#ifdef __cplusplus
}
#endif

#endif // OPENSUBC_CAPI_H
