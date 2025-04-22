package queue;
        // enqueue – добавить элемент в очередь;
        // element – первый элемент в очереди;
        // dequeue – удалить и вернуть первый элемент в очереди;
        // size – текущий размер очереди;
        // isEmpty – является ли очередь пустой;
        // clear – удалить все элементы из очереди.
public class TestArrayQueueADT {
    public static void main(String[] args) {
        ArrayQueueADT queue1 = new ArrayQueueADT();
        ArrayQueueADT queue2 = new ArrayQueueADT();

        System.out.println(ArrayQueueADT.size(queue1)); //0
        ArrayQueueADT.enqueue(queue1, "1");
        System.out.println(ArrayQueueADT.element(queue1)); //1
        System.out.println(ArrayQueueADT.size(queue1)); //1
        System.out.println(ArrayQueueADT.dequeue(queue1)); //1
        System.out.println(ArrayQueueADT.size(queue1)); //0

        System.out.println();

        System.out.println(ArrayQueueADT.size(queue2)); //0
        ArrayQueueADT.enqueue(queue2, "2");
        ArrayQueueADT.enqueue(queue2, "1");
        ArrayQueueADT.enqueue(queue2, "0");
        System.out.println(ArrayQueueADT.toStr(queue2)); //2
        System.out.println(ArrayQueueADT.size(queue2)); //1
        System.out.println(ArrayQueueADT.dequeue(queue2)); //2
        System.out.println(ArrayQueueADT.dequeue(queue2)); //2
        System.out.println(ArrayQueueADT.dequeue(queue2)); //2
        System.out.println(ArrayQueueADT.size(queue2)); //0
    }
}
