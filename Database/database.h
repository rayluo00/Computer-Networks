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


struct db_args {
	int DB_TYPE;
	char *DB_NAME;
};
typedef struct db_args db_args;

#define DATABASE_PROG 24670113
#define DATABASE_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define DB_START 1
extern  int * db_start_1(void *, CLIENT *);
extern  int * db_start_1_svc(void *, struct svc_req *);
#define DB_CREATE 2
extern  int * db_create_1(struct db_args *, CLIENT *);
extern  int * db_create_1_svc(struct db_args *, struct svc_req *);
#define DB_OPEN 3
extern  int * db_open_1(struct db_args *, CLIENT *);
extern  int * db_open_1_svc(struct db_args *, struct svc_req *);
#define DB_CLOSE 4
extern  int * db_close_1(void *, CLIENT *);
extern  int * db_close_1_svc(void *, struct svc_req *);
extern int database_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define DB_START 1
extern  int * db_start_1();
extern  int * db_start_1_svc();
#define DB_CREATE 2
extern  int * db_create_1();
extern  int * db_create_1_svc();
#define DB_OPEN 3
extern  int * db_open_1();
extern  int * db_open_1_svc();
#define DB_CLOSE 4
extern  int * db_close_1();
extern  int * db_close_1_svc();
extern int database_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_db_args (XDR *, db_args*);

#else /* K&R C */
extern bool_t xdr_db_args ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_DATABASE_H_RPCGEN */
