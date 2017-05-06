import java.util.*;
import java.net.URL;
import org.apache.xmlrpc.client.XmlRpcClient;
import org.apache.xmlrpc.client.XmlRpcClientConfigImpl;

public class db_client 
{
	public static void main (String[] args) 
	{
		int serverStatus;
		String command;
		String[] cmd;
		Scanner inScanner = new Scanner(System.in);

		try {
			XmlRpcClientConfigImpl config = new XmlRpcClientConfigImpl();
			config.setServerURL(new URL("http://localhost:"+args[1]+"/RPC2"));
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

				PerformCommand(cmd);
			}

		} catch (Exception exception) {
			System.out.println("error: "+exception);
		}
	}

	public static String[] ParseCommand (String command)
	{
		int i;
		String[] cmd = command.split(" ");
		String[] cmdArgs = cmd[1].split(",");

		for (i = 0; i < cmdArgs.length; i++) {
			cmd[i+1] = cmdArgs[i];
		}

		for (i = 0; i < cmd.length; i++) {
			System.out.println("CMD["+i+"]: "+cmd[i]);
		}

		return cmd;
	}

	public static void PerformCommand(String[] cmd) 
	{
		if (cmd[0].equals("q")) {
			System.exit(0);
		}
		else if (cmd[0].equals("get")) {

		}
	}
}
