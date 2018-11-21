package HTTP;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

public class HTTPClient {

    public static void main(String[] args) throws Exception {
        InetAddress addr = InetAddress.getLocalHost();
        Socket socket = new Socket (addr, 8080);
        PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
        BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        String cookie=null;

        out.println("GET / HTTP/1.1");
        out.println();
        out.println("Cookie"+cookie);

        while(true)
        {
            String str=in.readLine();

            String s = in.readLine();
            if(s==null) break;
            System.out.println(s);


        }
        socket.close();
    }
}
