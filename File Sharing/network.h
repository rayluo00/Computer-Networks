/********************************************************************
 * network.h
 *
 * Author: Raymond Weiming Luo
 *
 * Header files for the utiity functions in network.c. Utility
 * fucntions are used by all peers (leech, stream anbd seed).
 *
 */

int create_leech_socket(char *hostname, int port);
int create_seed_socket(int port);
void split_file(int argc, char **argv);
void merge_file(int user_count);
