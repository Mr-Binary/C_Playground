import java.io.*;
import java.net.*;
import java.util.*;

public class Client {
    public static void main(String[] args){
        Socket socket = null;
        try{
            int port = Integer.parseInt(args[0]);
            socket = new Socket(args[1], port);
            System.err.println("Connect to the server");
            new ReadWriteThread(System.in, socket.getOutputStream()).start();
            new ReadWriteThread(socket.getInputStream(), System.out).start();
        } catch (Exception e){
            e.printStackTrace();
        }
    }
}
