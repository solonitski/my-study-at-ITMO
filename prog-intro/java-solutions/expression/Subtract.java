package expression;

public class Subtract extends AbstractBinaryOperation {
    public Subtract(InterfaceExpression left, InterfaceExpression right) {
        super(left, right);
    }

    @Override
    public int countUp(int a, int b) {
        return a + (-1 * b);
    }

    @Override
    public String getOperation() {
        return " - ";
    }
}
