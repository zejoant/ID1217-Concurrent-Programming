package peertopeer;

public class Main{

    public static void main(String[] args) {
        int noOfStudents = args.length > 0 ? Integer.parseInt(args[0]) : 20; 
        int port = 8888;
        Student[] listOfStudents = new Student[noOfStudents];
        
        
        for (int i = 0; i<noOfStudents; i++){
            listOfStudents[i] = new Student(i, port+i);
            listOfStudents[i].start();
        }
        
        Teacher teacher = new Teacher(port+noOfStudents, noOfStudents);
        teacher.start();
    }
}



