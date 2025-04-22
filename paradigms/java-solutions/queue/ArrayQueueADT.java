package queue;

import java.util.Objects;

public class ArrayQueueADT {
    private final int DEFAULT_SIZE = 5;
    private Object[] elements = new Object[DEFAULT_SIZE];
    private int size = 0;
    private int head = 0;
    private int tail = 0;

    private static void increaseCapacity(ArrayQueueADT queue) {
        if ((queue.head == queue.tail && queue.elements[queue.head] != null)) {
            Object[] temp = new Object[queue.elements.length * 2];
            System.arraycopy(queue.elements, queue.head, temp, 0, queue.elements.length - queue.head);
            System.arraycopy(queue.elements, 0, temp, queue.elements.length - queue.head, queue.tail);
            queue.tail = queue.elements.length;
            queue.elements = temp;
            queue.head = 0;
        }
    }

    public static void enqueue(ArrayQueueADT queue, final Object element) {
        Objects.requireNonNull(element);
        queue.size++;
        increaseCapacity(queue);
        queue.elements[queue.tail] = element;
        queue.tail = (queue.tail + 1) % queue.elements.length;
    }

    public static Object element(ArrayQueueADT queue) {
        assert !isEmpty(queue);
        return queue.elements[queue.head];
    }

    public static Object dequeue(ArrayQueueADT queue) {
        assert !isEmpty(queue);
        Object element = element(queue);
        queue.size--;
        queue.elements[queue.head] = null;
        queue.head = (queue.head + 1) % queue.elements.length;
        return element;
    }

    public static int size(ArrayQueueADT queue) {
        return queue.size;
    }

    public static boolean isEmpty(ArrayQueueADT queue) {
        return queue.size(queue) == 0;
    }

    public static void clear(ArrayQueueADT queue) {
        queue.elements = new Object[queue.DEFAULT_SIZE];
        queue.size = 0;
        queue.head = 0;
        queue.tail = 0;
    }

    public static String toStr(ArrayQueueADT queue) {
        StringBuilder sb = new StringBuilder();
        sb.append('[');
        for (int i = 0; i < size(queue); i++) {
            sb.append(element(queue));
            if (i != size(queue) - 1) {
                sb.append(", ");
            }
            enqueue(queue, dequeue(queue));
        }
        sb.append(']');
        return sb.toString();
    }
}
