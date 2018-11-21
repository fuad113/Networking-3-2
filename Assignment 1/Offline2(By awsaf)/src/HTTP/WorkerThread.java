package HTTP;

import FileManagement.PostReq;
import FileManagement.Router;

import java.io.*;
import java.net.Socket;
import java.net.URL;
import java.util.HashMap;
import java.util.Scanner;

class WorkerThread implements Runnable
{
    private Socket socket;
    private InputStream is;
    private OutputStream os;

    private BufferedReader br;
    private PrintWriter pr;
    private File log;
    private FileWriter fileWriter;
    private int id;
    private String cookie;

    public void setcookie(String x)
    {
        this.cookie=x;
    }

    WorkerThread(Socket s, int id) throws IOException {
        this.socket = s;
        this.id = id;

        log = new File(HTTPServer.logdir+"/log."+id+".txt");
        if(!log.exists()){
            log.createNewFile();
            System.out.println("log file created");
        }

        try
        {
            this.is = this.socket.getInputStream();
            this.os = this.socket.getOutputStream();

            br = new BufferedReader(new InputStreamReader(this.is));
            pr = new PrintWriter(this.os);

        }
        catch(Exception e)
        {
            System.err.println("Sorry. Cannot manage client [" + id + "] properly.");
        }

    }

    public void run()
    {

//        try {
//            fileWriter = new FileWriter(log);
//        } catch (IOException e) {
//            e.printStackTrace();
//        }


        String str ="", httpRequest = "";
        char charIn;

        int counter=0;
        while(true)
        {
            counter++;
            try {

                StringBuilder buff = new StringBuilder();

                while (br.ready()){
                    charIn = (char) br.read();
                    buff.append(charIn);

                }
                str = buff.toString();
                //System.out.println("sizeeeeeeeeeeeeeeeeeeeeeeee : "+str.length());
                /*if(str!=null)
                {
                    fileWriter = new FileWriter(log);
                    fileWriter.write(str);
                    fileWriter.flush();
                    fileWriter.close();
                }*/


                Scanner sc = new Scanner(str);
                String url = null;

                while (sc.hasNextLine()){
                    String line = sc.nextLine();

                    System.out.println("[" + id + "] says: " + line);

                    String arr[] = line.split(" ");

                    if(arr[0].equals("GET")){
                        httpRequest = "GET";

                        try {



                            String url = "/index.html"+" " +;

                            File file = new File(url);
                            FileInputStream fis = null;

                            fis = new FileInputStream(file);

                            BufferedInputStream bis = new BufferedInputStream(fis);

                            byte[] contents;
                            long fileLength = file.length();

                            String head = "HTTP/1.1 200 OK\n" +
                                    "Date: Sun, 04 Nov 2018 11:50:15 GMT\n" +
                                    "Accept-Ranges: bytes\n" +
                                    "Content-Length: "+String.valueOf(fileLength)+"\n" +
                                    "Keep-Alive: timeout=15, max=100\n" +
                                    "Connection: Keep-Alive\n" +
                                    "Content-Type: "+getMIMEType(url)+"\n" +
                                    "CRLF";

//            System.out.println(head);
                            pr.println(head);
                            pr.flush();

                            pr.println("");
                            pr.flush();

                            pr.println(cookie);


                            long current = 0;

                            long start = System.nanoTime();
                            while(current!=fileLength){
                                int size = 10000;
                                if(fileLength - current >= size)
                                    current += size;
                                else{
                                    size = (int)(fileLength - current);
                                    current = fileLength;
                                }
                                contents = new byte[size];
                                bis.read(contents, 0, size);
                                os.write(contents);
                                System.out.println("Sending file ... "+(current*100)/fileLength+"% complete!");
                            }
                            os.flush();
                            System.out.println("File sent successfully!");

                        } catch (Exception e) {
                            e.printStackTrace();
                        }


                    }


                    else  if(arr[0].equals("POST")){
                        httpRequest = "POST";
                        url = arr[1];

                    }
                    else if(arr[0].equals("cookie"))
                    {
                        String temp= arr[1];
                        setcookie(temp);
                    }

                    else if(httpRequest.equals("POST") && arr[0].startsWith("user") && url != null){
                        String items[] = arr[0].split("&");
                        HashMap<String , String> map = new HashMap<>();
                        for (String item : items) {
                            String keyval[] = item.split("=");
                            if (keyval.length > 1) {
                                map.put(keyval[0], keyval[1]);
                            } else {
                                System.out.println("No value specified in input form.");
                                map.put(keyval[0], "");
                            }
                        }
                        Router r = new Router(url , os , br , map);
                        r.route("POST");

//                        PostReq pst = new PostReq(pr , os , url , map);
//                        pst.write();
                    }
                    else if(httpRequest.equals("POST") && arr[0].startsWith("Referer")){
                        String referer[] = arr[1].split("/");
                        url += "/"+referer[referer.length -1];
                    }
                }

            }
            catch(Exception e)
            {
                System.err.println("Problem in communicating with the client [" + id + "]. Terminating worker thread.");
                e.printStackTrace();
                break;
            }
        }

        try
        {
            this.is.close();
            this.os.close();
            this.socket.close();

        }
        catch(Exception e)
        {
            e.printStackTrace();
        }

        HTTPServer.workerThreadCount--;
        System.out.println("Client [" + id + "] is now terminating. No. of worker threads = "
                + HTTPServer.workerThreadCount);
    }







}