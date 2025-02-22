package serverclient;

import java.io.*;
import java.net.*;

public class Student extends Thread {
    int port = 0;
    int id = 0;
    String partnerId = null;
    String msg = null;

    public Student(int id, int port){
        this.id = id;
        this.port = port;
    }

    public void run(){
        try{
            Socket client = new Socket("localhost", port);
            DataOutputStream output = new DataOutputStream(client.getOutputStream());
            DataInputStream input = new DataInputStream(client.getInputStream());
            output.writeUTF(Integer.toString(id));
            while(true){
                msg = input.readUTF();
                if(msg.equals("Class is over! 🤓")){
                    if(id == Integer.parseInt(partnerId)){
                        System.out.println("I am student: " + id + " and I am alone :(");
                    }else{
                        System.out.println("I am student: " + id + ". My partner is student: " + partnerId);
                    }
                    break;
                }
                partnerId = msg;
            }
            client.close();
        }catch(Exception e) { }
    }
    
}
