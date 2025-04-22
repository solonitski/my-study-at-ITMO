package queue;
// enqueue – добавить элемент в очередь;
// element – первый элемент в очереди;
// dequeue – удалить и вернуть первый элемент в очереди;
// size – текущий размер очереди;
// isEmpty – является ли очередь пустой;
// clear – удалить все элементы из очереди.
public class TestArrayQueue {
    public static void main(String[] args) {
        ArrayQueue queue1 = new ArrayQueue();
        ArrayQueue queue2 = new ArrayQueue();

        // enqueue is OK.
        System.out.println("Size is " + queue1.size()); // 0
        System.out.println(queue1.toStr());
        queue1.enqueue("1");
        System.out.println("Size is " + queue1.size() + " " + queue1.head + " " + queue1.tail); // 1
        System.out.println(queue1.toStr());
        queue1.enqueue("2");
        System.out.println("Size is " + queue1.size()); // 2
        System.out.println(queue1.toStr());
        queue1.enqueue("3");
        System.out.println("Size is " + queue1.size()); // 3
        System.out.println(queue1.toStr());
        queue1.enqueue("4");
        System.out.println("Size is " + queue1.size()); // 4
        System.out.println(queue1.toStr());
        queue1.enqueue("5");
        System.out.println("Size is " + queue1.size()); // 5
        System.out.println(queue1.toStr());
        queue1.enqueue("6");
        System.out.println("Size is " + queue1.size()); // 6
        System.out.println(queue1.toStr());
        queue1.enqueue("7");
        System.out.println("Size is " + queue1.size()); // 7
        System.out.println(queue1.toStr());
        queue1.enqueue("8");
        System.out.println("Size is " + queue1.size()); // 8

        System.out.println("--------------------------------------");

        queue1.clear();
        queue1.enqueue("1");
        System.out.println("Size is " + queue1.size); // 1
        queue1.enqueue("2");
        System.out.println("Element is " + queue1.dequeue()); // 1
        queue1.enqueue("3");
        System.out.println("Element is " + queue1.dequeue()); // 2
        queue1.enqueue("4");
        System.out.println("Element is " + queue1.dequeue()); // 3
        queue1.enqueue("5");
        System.out.println("Element is " + queue1.dequeue()); // 4
        queue1.enqueue("6");
        System.out.println("Element is " + queue1.dequeue()); // 5
        queue1.enqueue("7");
        System.out.println("Element is " + queue1.dequeue()); // 6
        queue1.enqueue("8");
        System.out.println("Element is " + queue1.dequeue()); // 7
        System.out.println("Element is " + queue1.dequeue()); // 8

        System.out.println("--------------------------------------");

        queue1.clear();
        queue1.enqueue(1);
        queue1.enqueue(2);
        System.out.println(queue1.size());
        System.out.println(queue1.dequeue());
        System.out.println(queue1.size());
        System.out.println(queue1.dequeue());
        System.out.println(queue1.size());
    }
}
