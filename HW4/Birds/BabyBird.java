package Birds;
import java.util.Random;

public class BabyBird extends Thread {

    private Dish dish;
    private int id;
    Random rand = new Random();

    public BabyBird(Dish dish, int id){
        this.dish = dish;
        this.id = id;
    }

    public void run(){
        while(true){
            try{
                sleep(rand.nextInt(2000)+1000);
            } catch (InterruptedException ie){}
            dish.consume(id);
        }
    }
}
