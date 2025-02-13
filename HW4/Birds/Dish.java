package Birds;

public class Dish {
    private int worms = 0;

    public synchronized void consume(int id){
        while(worms == 0){
            try{
                wait();
            } catch(InterruptedException ie){}
        }
        worms--;
        System.out.println("Baby Bird #" + id + " eats a worm. Worms left: " + worms);
        if(worms < 1){
            System.out.println("BABY BIRD #" + id + " ALERTS PARENT");
            notifyAll();
        }
    }

    public synchronized void restock(int value){
        while(worms > 0){
           try{
            wait();
           } catch(InterruptedException ie){}
        }
        worms = value;
        System.out.println("Parent Bird restocks the worms, current worms: " + worms);
        notifyAll();
    }
    
}
