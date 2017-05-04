#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>

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

int main (int argc, char **argv)
{
	struct xmlrpc_method_info3 const method_info = {
		"sample.add",
		&sample_add,
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

	xmlrpc_registry_add_method3(&env, registry_ptr, &method_info);
	if (env.fault_occurred) {
		fprintf(stderr, "error: Unbale to add XML method.\n");
		exit(EXIT_FAILURE);
	}

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
