package search;

public class BinarySearchShift {
    //Contract
    //Pred: args.length >= 0: element = args[0] && 1 <= i < j < args.length: args[i] >= args[j] (downSort())
    //Post: R = index: array[index] <= element && 0 <= index < array.length
    public static void main(String[] args) {
        //Pred: length >= 0
        int[] array = new int[args.length];
        //Post: new int[] array && array.length >= 0 && array.length == args.length
        //Pred: True
        int parityCheck = 0;
        //Post: parityCheck = 0;
        for (int i = 0; i < args.length; i++) {
            parityCheck = (Integer.parseInt(args[i]) % 2) % 2;
            //parityCheck == (parityCheck' + args[i'] % 2) % 2
        }
        //length > 0
        for (int i = 0; i < args.length; i++) {
            //Pred: i' < length
            array[i] = Integer.parseInt(args[i]);
            //Post: array[i'] == args[i']
        }
        //args.length >= 0
        if (parityCheck == 0) {
            // parityCheck == 0
            System.out.println(recursiveBinarySearch(array, -1, array.length));
        } else {
            //parityCheck != 0
            System.out.println(iterativeBinarySearch(array));
        }
        //System.out.println(recursiveBinarySearch(array, -1, array.length));
        //System.out.println(iterativeBinarySearch(array));
    }

    //Contract
    //Pred: for all j = 1 to array.length -1 && j != i -> array[j] > array[j - 1], but exist array[i] < array[i - 1]
    //Post: Return int == i
    private static int iterativeBinarySearch(final int[] array) {
        if (array[0] > array[array.length - 1]) {
            // array is sorted -> shift == 0
            return 0;
        }
        //Pred: true
        int left = -1;
        //Post: left == -1
        //Pred: true
        int right = array.length;
        //Post: right == array.length
        // Pred: left < (right + left) / 2 < right
        int middle = (right + left) / 2;
        // Post: left < middle < right

        //inv1: array[m - 1] < array[m]
        while ((middle != array.length - 1) && !(middle > 0 && array[middle] > array[middle - 1])) {
            // Pred: Inv1 && left' < (right' + left') / 2 < right'
            middle = (right + left) / 2;
            // Post: Inv1 && left' < middle < right'
            if (array[0] >= array[middle]) {
                // Pred: Inv1 && middle = (right' + left') / 2 && array[middle] <= array[0]
                left = middle;
                // Pred: Inv1 && left = middle
            } else {
                // Pred: Inv1 && middle = (right' + left') / 2 && array[middle] > array[o]
                right = middle;
                // Pred: Inv1 && right = middle
            }
        }
        //Pred: array[0] >= array[array.length - 1] && 0 < middle < array.length -> middle == shifts
        return middle;
        //R = middle
    }

    //Contract
    //Pred: for all j = 1 to array.length -1 && j != i -> array[j] > array[j - 1], but exist array[i] < array[i - 1]
    // && left == -1 && right == array.length - 1
    //Post: Return int == i

    private static int recursiveBinarySearch(final int[] array, int left, int right) {
        if (array[0] > array[array.length - 1]) {
            // array is sorted -> shift == 0
            return 0;
        }
        // Pred: left < (right + left) / 2 < right
        int middle = (right + left) / 2;
        // Post: left < middle < right
        if (middle == array.length - 1) {
            //middle == (left + right) / 2
            // && middle == array.length - 1
            // && (array[middle] < array[middle - 1])
            return middle;
        }
        if (middle > 0 && array[middle] > array[middle - 1]) {
            //return was found.
            return middle;
        }
        //inv: return not found
        if (array[0] >= array[middle]) {
            // array[0] >= array[middle] && inv && middle == (right + left) / 2
            return recursiveBinarySearch(array, middle, right);
        } else {
            // array[0] < array[middle] && inv && middle == (right + left) / 2
            return recursiveBinarySearch(array, left, middle);
        }
    }
}
