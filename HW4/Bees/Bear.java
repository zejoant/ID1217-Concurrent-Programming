package Bees;
import java.util.Random;

public class Bear extends Thread{
    Random rand = new Random();
    private Pot pot;
    
    public Bear(Pot pot){
        this.pot = pot;
    }

    public void run(){
        while(true){
            try{
                sleep(2000);
            } catch (InterruptedException ie){}
            pot.empty();
        }
    }
}

