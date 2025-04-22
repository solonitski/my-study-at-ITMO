package queue;

public class LinkedQueue extends AbstractQueue {

    public static class Link {
        private Link next;
        private final Object value;
        public Link(Link next, Object value) {
            this.next = next;
            this.value = value;
        }
    }
    Link head = null;
    Link tail = null;
    @Override
    protected void enqueueImplement(Object element) {
        Link newLink = new Link(null, element);
        if (tail != null) {
            tail.next = newLink;
            tail = tail.next;
        } else {
            head = newLink;
            tail = newLink;
        }
    }
    @Override
    protected Object elementImplement() {
        return head.value;
    }
    @Override
    protected void dequeueImplement() {
        head = head.next;
        if (super.size == 0) {
            clear();
        }
    }
    @Override
    protected void clearImplement() {
        head = null;
        tail = null;
    }
}
