package peertopeer;

import java.io.*;
import java.net.*;
import java.util.*;

public class Teacher extends Thread{
    int noOfStudents;
    int port = 0;
    ArrayList<String> availableStudents = new ArrayList<String>();

    public Teacher(int port, int noOfStudents){
        this.port = port;
        this.noOfStudents = noOfStudents;
    }

    public void run(){
        Random rand = new Random();
        createArray(availableStudents);
        //System.out.println("teacher port " +port);
        try{
            Socket socket = new Socket("localhost", rand.nextInt(noOfStudents)+(port-noOfStudents));
            DataOutputStream output = new DataOutputStream(socket.getOutputStream());
            output.writeUTF("your turn to pick a partner" + "," + arrayListToString(availableStudents));
            socket.close();
        } catch(Exception e){ System.out.println("TEACHER" + e); }
    }

    public void createArray(ArrayList<String> arrayList){
        for(int i = 0; i<noOfStudents; i++){
            arrayList.add(Integer.toString(port-(i+1)));
            //System.out.println(arrayList.get(i));
        }
    }

    public String arrayListToString(ArrayList<String> arrayList){
        String string = "";
        for(int i = 0; i<arrayList.size(); i++){
            string += arrayList.get(i) + ",";
        }
        return string;
    }
}
