package expression;

public class Add extends AbstractBinaryOperation {

    public Add(InterfaceExpression left, InterfaceExpression right) {
        super(left, right);
    }

    @Override
    public int countUp(int a, int b) {
        return a + b;
    }

    @Override
    public String getOperation() {
        return " + ";
    }
}
