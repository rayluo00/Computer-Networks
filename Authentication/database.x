struct db_args {
	int DB_TYPE;
	string DB_NAME<>;
};

struct location_params {
	string NAME<>;       /* Unique key */
	string CITY<>;
	string STATE<>;
	string TYPE<>;
};

program DATABASE_PROG {
    version DATABASE_VERS {
        int DB_START (void) = 1;
        int DB_CREATE (struct db_args) = 2;
		int DB_OPEN (struct db_args) = 3;
		int DB_CLOSE (void) = 4;
		int DB_PUT (struct location_params) = 5;
		int DB_GET (struct location_params) = 6;
    } = 1;
} = 24670113;
