package peertopeer;

import java.io.*;
import java.net.*;
import java.util.*;

public class Student extends Thread {
    int port = 0;
    int id = 0;
    ArrayList<String> availableStudents = new ArrayList<String>();
    String msg = "";
    int randomInt = 0;
    String partnerId = "";
    DataInputStream input;
    DataOutputStream output;
    Socket peer;

    public Student(int id, int port){
        this.id = id;
        this.port = port;
    }

    
    public void run(){
        Random rand = new Random();
        try{
            ServerSocket sSocket = new ServerSocket(port);
            Socket peer = sSocket.accept();
            input = new DataInputStream(peer.getInputStream());
            while(true){
                msg = input.readUTF();
                if(msg != null){
                    availableStudents = stringToArrayList(msg);
                    if(availableStudents.get(0).equals("your turn to pick a partner")){

                        //if odd number of students, partner self
                        if(availableStudents.size()<3){
                            partnerId = Integer.toString(id);
                            break;
                        }

                        randomInt = rand.nextInt(availableStudents.size()-2)+1; //pick random partner id exlusive from self
                        availableStudents.remove(Integer.toString(port)); //remove self port

                        //new socket - contact peer to form group
                        peer = new Socket("localhost", Integer.parseInt(availableStudents.get(randomInt))); 
                        availableStudents.remove(randomInt); //remove partner port
                        output = new DataOutputStream(peer.getOutputStream());
                        input = new DataInputStream(peer.getInputStream());
                        output.writeUTF("Group?," + Integer.toString(id)); //request grouping 
                        while((partnerId = input.readUTF()) == null){} //wait for group partner to send id
                        peer.close();

                        if(availableStudents.size()>1){ //check if any students left
                            //new socket - contact peer to pick next group
                            randomInt = rand.nextInt(availableStudents.size()-1)+1;  //pick random partner id
                            peer = new Socket("localhost",Integer.parseInt(availableStudents.get(randomInt)));
                            output = new DataOutputStream(peer.getOutputStream());
                            output.writeUTF(arrayListToString(availableStudents)); //send list of available partners to group to next peer
                        }
                    }

                    //if peer has requested grouping
                    else if(availableStudents.get(0).equals("Group?")){
                        output = new DataOutputStream(peer.getOutputStream());
                        output.writeUTF(Integer.toString(id)); //give partner self id
                        partnerId = availableStudents.get(1); //save partner id 
                    }
                    break;
                }
            }
            peer.close();
        }catch(Exception e) { 
            System.out.println("STUDENT " + id +" "+ e); 
            for (int i = 0; i<availableStudents.size(); i++){
                System.out.print(availableStudents.get(i) + ", ");
            }
            System.out.println("");
        }

       System.out.println("I am student: " + id + " My partner is student: " + partnerId);
    }
    
    public String arrayListToString(ArrayList<String> arrayList){
        String string = "";
        for(int i = 0; i<arrayList.size(); i++){
            string += arrayList.get(i) + ",";
        }
        return string;
    }

    public ArrayList<String> stringToArrayList(String string){
        ArrayList<String> arrayList = new ArrayList<String>();
        String[] temp = string.split(",");
        for(int i = 0; i<temp.length; i++){
            arrayList.add(temp[i]);
        }
        return arrayList;
    }
}
