package expression;

public class Const implements InterfaceExpression {
    private final int constant;

    public Const(int constant) {
        this.constant = constant;
    }

    @Override
    public boolean equals(Object secondConstant) {
        if (secondConstant instanceof Const) {
            //:note: Too long, compare ints
            return this.toString().equals(secondConstant.toString());
        }
        return false;
    }
    @Override
    public int evaluate(int x, int y, int z) {
        return constant;
    }

    @Override
    public int evaluate(int constant) {
        return this.constant;
    }

    @Override
    public String toString() {
        return Integer.toString(constant);
    }

    @Override
    public int hashCode() {
        return Integer.hashCode(constant);
    }
}
