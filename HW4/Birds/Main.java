package Birds;

public class Main {
    public static void main(String[] args) {
        int numOfBabyBirds = args.length > 0 ? Integer.parseInt(args[0]) : 5;

        Dish dish = new Dish();
        ParentBird parentBird = new ParentBird(dish);
        BabyBird[] babyBirdArray = new BabyBird[numOfBabyBirds]; 

        for(int i = 0; i<numOfBabyBirds; i++){
           babyBirdArray[i] = new BabyBird(dish, i);
           babyBirdArray[i].start();
        }
        parentBird.start();
    }
}
