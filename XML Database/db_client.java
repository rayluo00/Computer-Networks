import java.util.*;
import java.net.URL;
import org.apache.xmlrpc.client.XmlRpcClient;
import org.apache.xmlrpc.client.XmlRpcClientConfigImpl;

public class db_client 
{
	public static void main (String[] args) 
	{
		String port = "46713";
		int serverStatus;
		String command;
		String[] cmd;
		Scanner inScanner = new Scanner(System.in);

		try {
			if (args.length > 0) {
				port = args[0];
			}

			XmlRpcClientConfigImpl config = new XmlRpcClientConfigImpl();
			config.setServerURL(new URL("http://localhost:"+port+"/RPC2"));
			XmlRpcClient server = new XmlRpcClient();
			server.setConfig(config);

			/*
			Object[] params = new Object[]{new Integer(5), new Integer(3)};

			serverStatus = (Integer) server.execute("sample.add", params);

			System.out.println("RETURN STATUS: "+serverStatus);
			*/

			while (true) {
				System.out.print("% ");
				command = inScanner.nextLine();

				cmd = ParseCommand(command);
				PerformCommand(server, cmd);
			}

		} catch (Exception exception) {
			System.out.println("error: "+exception);
		}
	}

	public static String[] ParseCommand (String command)
	{
		int i;
		String[] cmd = command.split(" ");

		for (i = 0; i < cmd.length; i++) {
			System.out.println("CMD["+i+"]: "+cmd[i]);
		}

		if (cmd[0].equals("q")) {
			System.exit(0);
		}

		return cmd;
	}

	public static void PerformCommand(XmlRpcClient server, String[] cmd) 
	{
		int serverStatus = 0;
		Object[] params;

		if (cmd.length < 2) {
			System.out.println("error: Missing parameters.");
			return;
		}

		try {
			
			params = new Object[]{new String(cmd[1])};

			if (cmd[0].equals("put")) {
				serverStatus = (Integer) server.execute("database.db_get", params);
			}
			else if (cmd[0].equals("get")) {
				serverStatus = (Integer) server.execute("database.db_get", params);
			}
			else if (cmd[0].equals("open")) {
				serverStatus = (Integer) server.execute("database.db_open", params);
			}
			else if (cmd[0].equals("close")) {
				serverStatus = (Integer) server.execute("database.db_close", params);

			}
			else if (cmd[0].equals("mk")) {
				serverStatus = (Integer) server.execute("database.db_create", params);
			}
		} catch (Exception exception) {
			System.out.println("error: "+exception);
		}
	}
}
