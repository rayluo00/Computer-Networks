import java.util.*;
import java.net.URL;
import org.apache.xmlrpc.client.XmlRpcClient;
import org.apache.xmlrpc.client.XmlRpcClientConfigImpl;

public class db_client 
{
	public static void main (String[] args) 
	{
		try {
			XmlRpcClientConfigImpl config = new XmlRpcClientConfigImpl();
			config.setServerURL(new URL("http://localhost:8080/RPC2"));
			XmlRpcClient server = new XmlRpcClient();
			server.setConfig(config);

			Object[] params = new Object[]{new Integer(5), new Integer(3)};

			int serverStatus = (Integer) server.execute("sample.add", params);

			System.out.println("RETURN STATUS: "+serverStatus);

		} catch (Exception exception) {
			System.out.println("error: "+exception);
		}
	}
}
