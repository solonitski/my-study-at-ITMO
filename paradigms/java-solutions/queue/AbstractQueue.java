package queue;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public abstract class AbstractQueue implements Queue {
    protected int size = 0;
    protected abstract void enqueueImplement(final Object element);

    protected abstract Object elementImplement();

    protected abstract void dequeueImplement();

    protected abstract void clearImplement();

    public void enqueue(final Object element) {
        Objects.requireNonNull(element);
        size++;
        enqueueImplement(element);
    }

    public Object element() {
        assert !isEmpty();
        return elementImplement();
    }

    public Object dequeue() {
        assert !isEmpty();
        size--;
        Object result = elementImplement();
        dequeueImplement();
        return result;
    }

    public int size() {
        return size;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public void clear() {
        size = 0;
        clearImplement();
    }
    public String toStr() {
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
    // Pred: true
    // Post: count of element in queue.
    public int count(Object element) {
        Objects.requireNonNull(element);
        int count = 0;
        for (int i = 0; i < size(); i++) {
            Object temp = element();
            if (element.equals(temp)) {
                count++;
            }
            enqueue(dequeue());
        }
        return count;
    }
}
