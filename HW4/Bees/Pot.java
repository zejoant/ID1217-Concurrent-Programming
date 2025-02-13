package Bees;

public class Pot {
    private int honey = 0;
    private int full;

    public Pot(int full){
        this.full = full;
    }

    public synchronized void add(int id){
        while(honey == full){
            try{
                wait();
            } catch(InterruptedException ie){}
        }
        honey++;
        System.out.println("Bee #" + id + " adds honey to pot. current pot: " + honey);
        if(honey == full){
            System.out.println("Bee #" + id + " AWAKENS BEAR"); 
            notifyAll();
        }
    }

    public synchronized void empty(){
        while(honey != full){
           try{
            wait();
           } catch(InterruptedException ie){}
        }
        honey = 0;
        System.out.println("Bear eats all the honey and then goes back to sleep");
        notifyAll();
    }
    
}
