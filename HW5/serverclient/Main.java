package serverclient;

public class Main{

    public static void main(String[] args) {
        int noOfStudents = args.length > 0 ? Integer.parseInt(args[0]) : 20; 
        int port = 8888;
        
        Teacher teacher = new Teacher(port, noOfStudents);

        teacher.start();

        for (int i = 0; i<noOfStudents; i++){
            new Student(i, port).start();
        }
    }
}



