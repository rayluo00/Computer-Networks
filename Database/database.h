/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _DATABASE_H_RPCGEN
#define _DATABASE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define DATABASE_PROG 24670113
#define DATABASE_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define DB_START 1
extern  int * db_start_1(void *, CLIENT *);
extern  int * db_start_1_svc(void *, struct svc_req *);
#define DB_OPTION 2
extern  int * db_option_1(int *, CLIENT *);
extern  int * db_option_1_svc(int *, struct svc_req *);
extern int database_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define DB_START 1
extern  int * db_start_1();
extern  int * db_start_1_svc();
#define DB_OPTION 2
extern  int * db_option_1();
extern  int * db_option_1_svc();
extern int database_prog_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_DATABASE_H_RPCGEN */
