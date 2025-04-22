package queue;

public class TestArrayQueueModule {
    public static void main(String[] args) {
        ArrayQueueModule queue = new ArrayQueueModule();
        System.out.println(ArrayQueueModule.size()); //0
        ArrayQueueModule.enqueue("1");
        ArrayQueueModule.enqueue("2");
        ArrayQueueModule.enqueue("1");
        ArrayQueueModule.enqueue("2");

        System.out.println(ArrayQueueModule.element()); //1

        System.out.println(ArrayQueueModule.size()); //1

        System.out.println(ArrayQueueModule.dequeue()); //1

        ArrayQueueModule.clear();

        System.out.println(ArrayQueueModule.size()); //0
        System.out.println(ArrayQueueModule.toStr());
        ArrayQueueModule.enqueue("3");
        System.out.println(ArrayQueueModule.toStr());
        System.out.println(ArrayQueueModule.dequeue());//1
        ArrayQueueModule.enqueue("3");
        ArrayQueueModule.enqueue("3");

        System.out.println(ArrayQueueModule.toStr());
    }
}
