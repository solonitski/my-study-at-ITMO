package search;

public class BinarySearch {
    //Contract
    //Pred: args.length >= 0: element = args[0] && 1 <= i < j < args.length: args[i] >= args[j] (downSort())
    //Post: R = index: array[index] <= element && 0 <= index < array.length
    public static void main(String[] args) {
        //Pred: args.length > 0
        int length = args.length - 1;
        //Post: length >= 0
        //Pred: length >= 0
        int[] array = new int[length];
        //Post: new int[] array && length >= 0
        //Pred: args.length > 0
        int element = Integer.parseInt(args[0]);
        //Post: element = args[0];
        //Pred: True
        int parityCheck = 0;
        //Post: parityCheck = 0;
        for (int i = 0; i < args.length; i++) {
            parityCheck = (Integer.parseInt(args[i]) % 2) % 2;
            //parityCheck == (parityCheck' + args[i'] % 2) % 2
        }
        //length > 0
        for (int i = 0; i < length; i++) {
            //Pred: i' < length
            array[i] = Integer.parseInt(args[i + 1]);
            //Post: array[i'] == args[i' + i]
        }
        //args.length >= 0 && element == args[0]
        if (parityCheck == 0) {
            // parityCheck == 0
            System.out.println(recursiveBinarySearch(element, array, -1, array.length));
        } else {
            //parityCheck != 0
            System.out.println(iterativeBinarySearch(element, array));
        }
    }

    // INV: for all i,j i>j: arr[i] <= arr[j] && arr[-1] == -∞ && arr[arr.length] == +∞

    //Contract
    //Pred: INV
    //Post: R == MIN(arr[i] <= x).
    private static int iterativeBinarySearch(final int element, final int[] array) {
        //Pred: true
        int left = -1;
        //Post: left == -1
        //Pred: true
        int right = array.length;
        //Post: right == array.length

        // Inv1: array[left] > element && array[right] <= element && left <= left' && right' <= right
        // && right >= 0 && right' < array.length
        // && left >= -1 && left < array.length && right - left > 1
        while (right - left > 1) {
            // Pred: Inv1 && left' < (right' + left') / 2 < right'
            int middle = (right + left) / 2;
            // Post: Inv1 && left' < middle < right'
            if (element < array[middle]) {
                // Pred: Inv1 && middle = (right' + left') / 2 && array[middle] > element
                left = middle;
                // Pred: Inv1 && left = middle && array[middle] > element
            } else {
                // Pred: Inv1 && middle = (right' + left') / 2 && array[middle] <= element
                right = middle;
                // Pred: Inv1 && left = middle && array[middle] <= element
            }
        }
        //Pred: right - left <= 1 && 0 <= right < array.length -> array[right] <= element
        return right;
        //R = right
    }

    //Contract
    //Inv: for all i,j i>j: arr[i] <= arr[j] && arr[-1] == -∞ && arr[arr.length] == +∞
    //&& -1 <= l < array.length && 0 <= right <= array.length
    //Post: R == MIN(arr[i] <= x).
    private static int recursiveBinarySearch(final int element, final int[] array, int left, int right) {
        if (right - left <= 1) {
            //Pred: right - left <= 1 && Inv && 0 <= right <= array.length
            return right;
            // R = right: array[right] == element && 0 <= right < array.length
        }
        // Pred: Inv && left' < (right' + left') / 2 < right'
        int middle = (right + left) / 2;
        // Post: Inv && left' < middle < right'
        if (element < array[middle]) {
            //Pred: element < array[middle] && Inv && left < middle < right
            return recursiveBinarySearch(element, array, middle, right);
            //Post: R = recursiveBinarySearch(element, array, (left + right) / 2, right)
        } else {
            //Pred: element >= array[middle] && Inv && left < middle < right
            return recursiveBinarySearch(element, array, left, middle);
            //Post: R = recursiveBinarySearch(element, array, left, (left + right) / 2)
        }
    }
}
