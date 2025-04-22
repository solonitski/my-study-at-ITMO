package expression;

public class Multiply extends AbstractBinaryOperation {
    public Multiply(InterfaceExpression left, InterfaceExpression right) {
        super(left, right);
    }

    @Override
    public String getOperation() {
        return " * ";
    }

    @Override
    public int countUp(int x, int y) {
        return x * y;
    }
}
