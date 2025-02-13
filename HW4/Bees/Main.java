package Bees;

public class Main {
    public static void main(String[] args) {
        int numOfBees = args.length > 0 ? Integer.parseInt(args[0]) : 5;
        int full = args.length > 1 ? Integer.parseInt(args[1]) : 10;

        Pot pot = new Pot(full);
        Bear bear = new Bear(pot);
        Bee[] beeArray = new Bee[numOfBees]; 

        for(int i = 0; i<numOfBees; i++){
            beeArray[i] = new Bee(pot, i);
            beeArray[i].start();
        }
        bear.start();
    }
}
