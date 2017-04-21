/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "database.h"

bool_t
xdr_db_args (XDR *xdrs, db_args *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->DB_TYPE))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->DB_NAME, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_location_params (XDR *xdrs, location_params *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->NAME, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->CITY, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->STATE, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->TYPE, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->STATUS))
		 return FALSE;
	return TRUE;
}
