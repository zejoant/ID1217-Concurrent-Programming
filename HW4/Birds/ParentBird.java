package Birds;
import java.util.Random;

public class ParentBird extends Thread{
    Random rand = new Random();
    private Dish dish;
    
    public ParentBird(Dish dish){
        this.dish = dish;
    }

    public void run(){
        while(true){
            try{
                sleep(2000);
            } catch (InterruptedException ie){}
            dish.restock(rand.nextInt(10)+10);
        }
    }
}

