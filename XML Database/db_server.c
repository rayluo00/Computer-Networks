#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>
#include <gdbm.h>

struct location_params {
	char *name;
	char *city;
	char *state;
	char *type;
	int valid;
};

// Gdbm Database
GDBM_FILE DATABASE;

/********************************************************************
 *
 */
struct location_params fill_location_params (char *params) {
	struct location_params loc_params;
	char *token;
	char name[256];
	char city[256];
	char state[256];
	char type[256];
	int i = 0;
	int tok_len;

	memset(name, '\0', sizeof(name));
	memset(city, '\0', sizeof(city));
	memset(state, '\0', sizeof(state));
	memset(type, '\0', sizeof(type));

	while ((token = strsep(&params, ",")) != NULL) {
		//printf("TOKEN[%d]: %s\n", i, token);
		tok_len = strlen(token);

		switch (i) {
			case 0:
				strncpy(name, token, tok_len);
				break;
			case 1:
				strncpy(city, token, tok_len);
				break;
			case 2:
				strncpy(state, token, tok_len);
				break;
			case 3:
				strncpy(type, token, tok_len);
				break;
			default:
				fprintf(stderr, "error: Invalid location params %s\n", token);
				loc_params.valid = -1;
				return loc_params;
		}

		i++;
	}

	if (i != 4) {
		loc_params.valid = -1;
		fprintf(stderr, "error: Missing location parameters.\n");
		return loc_params;
	}

	loc_params.name = name;
	loc_params.city = city;
	loc_params.state = state;
	loc_params.type = type;
	loc_params.valid = 0;

	return loc_params;
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
		fprintf(stderr, "error: Failed to decompose XML value.\n");
		return xmlrpc_build_value(env, "i", -1);
	}

	printf("CREATE %s\n", input);

	DATABASE = gdbm_open(input, 0, GDBM_WRCREAT, 0644, 0);

	if (DATABASE == NULL) {
		fprintf(stderr, "error: Failed to create GDBM.\n");
		return xmlrpc_build_value(env, "i", -1);
	}

	gdbm_close(DATABASE);

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
		fprintf(stderr, "error: Failed to decompose XML value.\n");
		return xmlrpc_build_value(env, "i", -1);
	}

	printf("OPEN %s\n", input);

	DATABASE = gdbm_open(input, 0, GDBM_WRITER, 0644, 0);

	if (DATABASE == NULL) {
		return xmlrpc_build_value(env, "i", -1);
	}

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
		fprintf(stderr, "error: Failed to decompose XML value.\n");
		return xmlrpc_build_value(env, "i", -1);
	}

	printf("CLOSE %s\n", input);

	gdbm_close(DATABASE);
	DATABASE = NULL;

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
	struct location_params loc_params;

	if (DATABASE == NULL) {
		fprintf(stderr, "error: No opened database.\n");
		return xmlrpc_build_value(env, "i", -1);
	}

	xmlrpc_decompose_value(env, param_array, "(s)", &input);
	if (env->fault_occurred) {
		fprintf(stderr, "error: Failed to decompose XML value.\n");
		return xmlrpc_build_value(env, "i", -1);
	}

	loc_params = fill_location_params(input);

	if (loc_params.valid < 0) {
		fprintf(stderr, "error: Invalid location struct creation.\n");
		return xmlrpc_build_value(env, "i", -1);
	}

	printf("PUT %s | %s,%s,%s,%s\n", input, loc_params.name, loc_params.city, loc_params.state, loc_params.type);

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
	struct location_params loc_params;

	if (DATABASE == NULL) {
		fprintf(stderr, "error: No opened database.\n");
		return xmlrpc_build_value(env, "i", -1);
	}

	xmlrpc_decompose_value(env, param_array, "(s)", &input);
	if (env->fault_occurred) {
		fprintf(stderr, "error: Failed to decompose XML value.\n");
		return xmlrpc_build_value(env, "i", -1);
	}

	loc_params = fill_location_params(input);

	if (loc_params.valid < 0) {
		fprintf(stderr, "error: Invalid location struct creation.\n");
		return xmlrpc_build_value(env, "i", -1);
	}
	
	printf("GET %s | %s,%s,%s,%s\n", input, loc_params.name, loc_params.city, loc_params.state, loc_params.type);

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
