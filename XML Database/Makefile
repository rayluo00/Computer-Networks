XMLRPC_C_CONFIG = xmlrpc-c-config
INCLUDES = $(shell $(XMLRPC_C_CONFIG) abyss-server --cflags)
LIBS = $(shell $(XMLRPC_C_CONFIG) abyss-server --libs)
CLNT_JAR = "./apache-xmlrpc-3.1.3/lib/xmlrpc-client-3.1.3.jar"
WS_JAR = "./apache-xmlrpc-3.1.3/lib/ws-commons-util-1.0.2.jar"
COMMON_JAR = "./apache-xmlrpc-3.1.3/lib/xmlrpc-common-3.1.3.jar"
LOG_JAR = "./apache-xmlrpc-3.1.3/lib/commons-logging-1.1.jar"
SVR_JAR = "./apache-xmlrpc-3.1.3/lib/xmlrpc-server-3.1.3.jar"

make:
	gcc -g -lgdbm -c $(INCLUDES) db_server.c
	gcc -g -lgdbm -o svr $(LIBS) db_server.o
	javac -Xlint -cp $(CLNT_JAR):$(COMMON_JAR):$(WS_JAR):$(LOG_JAR):$(SVR_JAR) db_client.java

run:
	java -cp .:$(CLNT_JAR):$(COMMON_JAR):$(WS_JAR):$(LOG_JAR):$(SVR_JAR) db_client 46713

clean:
	$(RM) *.o, svr , *~ , *.gch , *.class
