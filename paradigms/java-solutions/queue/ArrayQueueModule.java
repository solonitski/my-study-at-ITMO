package queue;

import java.util.Objects;

public class ArrayQueueModule {
    private static final int DEFAULT_SIZE = 5;
    private static Object[] elements = new Object[DEFAULT_SIZE];
    private static int size = 0;
    private static int head = 0;
    private static int tail = 0;

    private static void increaseCapacity() {
        if ((head == tail && elements[head] != null)) {
            Object[] temp = new Object[elements.length * 2];
            System.arraycopy(elements, head, temp, 0, elements.length - head);
            System.arraycopy(elements, 0, temp, elements.length - head, tail);
            tail = elements.length;
            elements = temp;
            head = 0;
        }
    }

    public static void enqueue(final Object element) {
        Objects.requireNonNull(element);
        size++;
        increaseCapacity();
        elements[tail] = element;
        tail = (tail + 1) % elements.length;
    }

    public static Object element() {
        assert !isEmpty();
        return elements[head];
    }

    public static Object dequeue() {
        assert !isEmpty();
        Object element = element();
        size--;
        elements[head] = null;
        head = (head + 1) % elements.length;
        return element;
    }

    public static int size() {
        return size;
    }

    public static boolean isEmpty() {
        return size() == 0;
    }

    public static void clear() {
        elements = new Object[DEFAULT_SIZE];
        size = 0;
        head = 0;
        tail = 0;
    }

    public static String toStr() {
        StringBuilder sb = new StringBuilder();
        sb.append('[');
        for (int i = 0; i < size(); i++) {
            sb.append(element());
            if (i != size() - 1) {
                sb.append(", ");
            }
            enqueue(dequeue());
        }
        sb.append(']');
        return sb.toString();
    }
}
