package FileManagement;

import java.io.*;
import java.util.HashMap;

public class Router {

    private String url;

    private OutputStream os;

    private PrintWriter pr;
    private BufferedReader br;
    private HashMap<String , String> map;


    public Router(String url, OutputStream os, BufferedReader br) {
        this.url = url;
        this.os = os;
        this.br = br;

        pr = new PrintWriter(os);
    }

    public Router(String url, OutputStream os, BufferedReader br, HashMap<String, String> map) {
        this.url = url;
        this.os = os;
        this.br = br;
        this.map = map;

        pr = new PrintWriter(os);
    }


    public boolean FileExists(String url){
        File f = new File(url);
        return f.exists();
    }


    public void route(String request) {
        if(url.endsWith("/")){
            url += "index.html";
        }
        else if(!url.contains(".")){
            url += "/index.html";
        }

        url = url.substring(1);

        if(! FileExists(url)){
            System.out.println("NOT FOUND in Server" +url);
            NotFound nf = new NotFound(pr, os);
            nf.write();
        }
        else if(request.equals("POST")){
            PostReq p = new PostReq(pr , os, url, map);
            p.write();

        }
        else if(request.equals("GET")){
            System.out.println("File path ->" +url);

            HttpSend f = new HttpSend(pr,os ,url);
            f.write();
        }
        else {
            System.out.println("Unknown Request");
        }


    }
}
