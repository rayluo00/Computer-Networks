struct db_args {
	int DB_TYPE;
	char *DB_NAME;
};

program DATABASE_PROG {
    version DATABASE_VERS {
        int DB_START (void) = 1;
        int DB_CREATE (struct db_args) = 2;
    } = 1;
} = 24670113;
