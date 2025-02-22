package serverclient;

import java.io.*;
import java.net.*;

public class Teacher extends Thread{
    int noOfStudents = 0;
    int port = 0;
    DataOutputStream students[];
    String indexA = null;
    String indexB = null;

    public Teacher(int port, int noOfStudents){
        this.port = port;
        this.noOfStudents = noOfStudents;
        students = new DataOutputStream[noOfStudents];
    }

    public void run(){
        DataInputStream input = null;
        DataOutputStream output = null;
        try{
            ServerSocket server = new ServerSocket(port);
            for(int i = 0; i<noOfStudents; i++){
                Socket client = server.accept();
                input = new DataInputStream(client.getInputStream());
                output = new DataOutputStream(client.getOutputStream()); 
                if(indexA == null){
                    students[i] = output;
                    while(true){
                        if((indexA = input.readUTF()) != null){
                            break;
                        }
                    }
                    if(i == noOfStudents-1){
                        output.writeUTF(indexA);
                        break;
                    }
                }else if(indexB == null){
                    students[i] = output;
                    while(true){
                        if((indexB = input.readUTF()) != null){
                            break;
                        }
                    }
                    students[i-1].writeUTF(indexB);
                    students[i].writeUTF(indexA);
                    indexA = null; indexB = null;  
                }
            }
            for(int i = noOfStudents-1; i>=0; i--){
                students[i].writeUTF("Class is over! 🤓");
            }
            server.close();
        }catch(Exception e){ }
    }
}
