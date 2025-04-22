package expression;

public class Divide extends AbstractBinaryOperation{
    public Divide(InterfaceExpression left, InterfaceExpression right) {
        super(left, right);
    }

    @Override
    public String getOperation() {
        return " / ";
    }

    @Override
    public int countUp(int x, int y) {
        return x / y;
    }
}
