#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>
#include <gdbm.h>

// Gdbm Database
GDBM_FILE DATABASE;

static xmlrpc_value *sample_add (xmlrpc_env *env, 
								 xmlrpc_value *param_array,
								 void *server_info,
								 void *channel_info)
{
	xmlrpc_int32 x, y, z;

	xmlrpc_decompose_value(env, param_array, "(ii)", &x, &y);
	if (env->fault_occurred) {
		return NULL;
	}

	z = x + y;

	printf("%d[x] + %d[y] = %d[z]\n", x, y, z);

	return xmlrpc_build_value(env, "i", z);
}

/********************************************************************
 *
 */
static xmlrpc_value *db_create (xmlrpc_env *env, 
								xmlrpc_value *param_array,
								void *server_info,
								void *channel_info)
{
	char *input;

	xmlrpc_decompose_value(env, param_array, "(s)", &input);
	if (env->fault_occurred) {
		return NULL;
	}

	printf("CREATE %s\n", input);

	return xmlrpc_build_value(env, "i", 0);
}

/********************************************************************
 *
 */
static xmlrpc_value *db_open (xmlrpc_env *env, 
							  xmlrpc_value *param_array,
							  void *server_info,
							  void *channel_info)
{
	char *input;

	xmlrpc_decompose_value(env, param_array, "(s)", &input);
	if (env->fault_occurred) {
		return NULL;
	}

	printf("OPEN %s\n", input);

	return xmlrpc_build_value(env, "i", 0);
}

/********************************************************************
 *
 */
static xmlrpc_value *db_close (xmlrpc_env *env, 
							   xmlrpc_value *param_array,
							   void *server_info,
							   void *channel_info)
{
	char *input;

	xmlrpc_decompose_value(env, param_array, "(s)", &input);
	if (env->fault_occurred) {
		return NULL;
	}

	printf("CLOSE %s\n", input);

	return xmlrpc_build_value(env, "i", 0);
}

/********************************************************************
 *
 */
static xmlrpc_value *db_put (xmlrpc_env *env, 
							 xmlrpc_value *param_array,
							 void *server_info,
							 void *channel_info)
{
	char *input;

	xmlrpc_decompose_value(env, param_array, "(s)", &input);
	if (env->fault_occurred) {
		return NULL;
	}

	printf("PUT %s\n", input);

	return xmlrpc_build_value(env, "i", 0);
}

/********************************************************************
 *
 */
static xmlrpc_value *db_get (xmlrpc_env *env, 
							 xmlrpc_value *param_array,
							 void *server_info,
							 void *channel_info)
{
	char *input;

	xmlrpc_decompose_value(env, param_array, "(s)", &input);
	if (env->fault_occurred) {
		return NULL;
	}

	printf("GET %s\n", input);

	return xmlrpc_build_value(env, "i", 0);
}

/********************************************************************
 *
 */
void register_xmlrpc (xmlrpc_env env, xmlrpc_registry *registry_ptr, 
					  struct xmlrpc_method_info3 method)
{
	xmlrpc_registry_add_method3(&env, registry_ptr, &method);

	if (env.fault_occurred) {
		fprintf(stderr, "error: Unbale to add XML method.\n");
		exit(EXIT_FAILURE);
	}
}

/********************************************************************
 *
 */
int main (int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "error: Missing port number.\n");
		exit(EXIT_FAILURE);
	}

	struct xmlrpc_method_info3 const open_method = {
		"database.db_open",
		&db_open,
	};

	struct xmlrpc_method_info3 const close_method = {
		"database.db_close",
		&db_close,
	};

	struct xmlrpc_method_info3 const get_method = {
		"database.db_get",
		&db_get,
	};

	struct xmlrpc_method_info3 const put_method = {
		"database.db_put",
		&db_put,
	};

	struct xmlrpc_method_info3 const create_method = {
		"database.db_create",
		&db_create,
	};

	xmlrpc_server_abyss_parms server_parms;
	xmlrpc_registry *registry_ptr;
	xmlrpc_env env;

	if (argc < 1) {
		fprintf(stderr, "error: Missing TCP port number.\n");
		exit(EXIT_FAILURE);
	}

	xmlrpc_env_init(&env);

	registry_ptr = xmlrpc_registry_new(&env);
	if (env.fault_occurred) {
		fprintf(stderr, "error: Unable to register the XML server.\n");
		exit(EXIT_FAILURE);
	}

	// TODO Create method to automatically register the XML methods
	/*
	xmlrpc_registry_add_method3(&env, registry_ptr, &open_method);
	if (env.fault_occurred) {
		fprintf(stderr, "error: Unbale to add XML method.\n");
		exit(EXIT_FAILURE);
	}
	*/

	register_xmlrpc(env, registry_ptr, open_method);
	register_xmlrpc(env, registry_ptr, close_method);
	register_xmlrpc(env, registry_ptr, create_method);
	register_xmlrpc(env, registry_ptr, put_method);
	register_xmlrpc(env, registry_ptr, get_method);

	server_parms.config_file_name = NULL;
	server_parms.registryP = registry_ptr;
	server_parms.port_number = atoi(argv[1]);
	server_parms.log_file_name = "/tmp/xmlrpc_log";

	printf("START Database\n");

	xmlrpc_server_abyss(&env, &server_parms, XMLRPC_APSIZE(log_file_name));
	if (env.fault_occurred) {
		fprintf(stderr, "error: Server abyss failed.\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
