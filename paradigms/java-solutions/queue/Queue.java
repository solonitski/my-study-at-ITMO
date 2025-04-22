package queue;
// Model: array[0]...array[n - 1]
// Invariant: n >= 0 && for i = 0 to n - 1 -> array[i] != null
public interface Queue {
    // Pred: element != null
    // Post: n' = n + 1 && array[n'] = element && for i = 0 to n - 1  -> n: array'[i] = array[i]
    void enqueue(Object element);
    // Pred: n > 0
    // Post: R = array[0] (first element in queue)
    Object element();
    // Pred: n > 0
    // Post: R = array[0] (first element in queue) &&
    // array = {array[1] ... array[n - 1]}
    Object dequeue();
    // Pred: true
    // Post: R = n (length)
    int size();
    // Pred: true
    // Post: R = (n == 0)
    boolean isEmpty();
    // Pred: true
    // Post: n = 0; for i = 0 to n - 1 -> array[i] = null
    void clear();
    // Pred: true
    // Post: String : [array[0], ..., array[n - 1]]
    String toStr();
}
