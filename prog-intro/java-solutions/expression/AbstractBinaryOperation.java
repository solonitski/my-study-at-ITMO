package expression;

import java.util.Objects;

public abstract class AbstractBinaryOperation implements InterfaceExpression {
    private final InterfaceExpression left;
    private final InterfaceExpression right;
    
    protected AbstractBinaryOperation(InterfaceExpression left, InterfaceExpression right) {
        this.left = left;
        this.right = right;
    }
    
    @Override
    public String toString() {
        return "(" + left + getOperation() + right + ")";
    }
    
    public abstract String getOperation();
    
    @Override
    public int evaluate(int x) {
        return countUp(left.evaluate(x), right.evaluate(x));
    }
    
    public abstract int countUp(int x, int y);
    
    @Override
    public int evaluate(int x, int y, int z) {
        return countUp(left.evaluate(x, y, z), right.evaluate(x, y, z));
    }
    
    @Override
    public boolean equals(Object object) {
        if (object instanceof AbstractBinaryOperation temp) {
            return Objects.equals(getClass(),
                    object.getClass()) && Objects.equals(left, temp.left) && Objects.equals(right, temp.right);
        }
        return false;
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(left.hashCode() * 997, right.hashCode() * 1337, getOperation().hashCode() * 83);
    }
}
