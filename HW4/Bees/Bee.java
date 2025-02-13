package Bees;
import java.util.Random;

public class Bee extends Thread {

    private Pot pot;
    private int id;
    Random rand = new Random();

    public Bee(Pot pot, int id){
        this.pot = pot;
        this.id = id;
    }

    public void run(){
        while(true){
            try{
                sleep(rand.nextInt(2000)+1000);
            } catch (InterruptedException ie){}
            pot.add(id);
        }
    }
}
