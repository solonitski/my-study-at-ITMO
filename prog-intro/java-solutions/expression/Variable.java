package expression;

public class Variable implements InterfaceExpression {
    private final String varName;

    public Variable(String varName) {
        this.varName = varName;
    }

    @Override
    public int evaluate(int number) {
        return number;
    }

    //:note: exception on otherwise than z
    @Override
    public int evaluate(int x, int y, int z) {
        switch (varName) {
            case "x":
                return x;
            case "y":
                return y;
            default:
                return z;
        }
    }

    @Override
    public boolean equals(Object secondVar) {
        if (secondVar instanceof Variable) {
            return this.toString().equals(secondVar.toString());
        }
        return false;
    }

    @Override
    public String toString() {
        return varName;
    }

    @Override
    public int hashCode() {
        return varName.hashCode();
    }
}
