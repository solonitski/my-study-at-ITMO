package queue;

import java.util.Objects;
public class ArrayQueue extends AbstractQueue {
    private final int DEFAULT_SIZE = 5;
    private Object[] elements = new Object[DEFAULT_SIZE];
    protected int head = 0;
    protected int tail = 0;

    private void increaseCapacity() {
        if ((head == tail && elements[head] != null)) {
            Object[] temp = new Object[elements.length * 2];
            System.arraycopy(elements, head, temp, 0, elements.length - head);
            System.arraycopy(elements, 0, temp, elements.length - head, tail);
            tail = elements.length;
            elements = temp;
            head = 0;
        }
    }
    @Override
    protected void enqueueImplement(Object element) {
        increaseCapacity();
        elements[tail] = element;
        tail = (tail + 1) % elements.length;
    }

    @Override
    public Object elementImplement() {
        return elements[head];
    }
    @Override
    public void dequeueImplement() {
        elements[head] = null;
        head = (head + 1) % elements.length;
    }

    @Override
    public void clearImplement() {
        elements = new Object[DEFAULT_SIZE];
        head = tail = 0;
    }
}
