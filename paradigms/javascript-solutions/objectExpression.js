"use strict"
function newExpression(expression, evaluate, toString) {
    expression.prototype.evaluate = evaluate;
    expression.prototype.toString = toString;
}

const Const = function (value) {
    this.value = value;
}
newExpression(Const,
    function () {
        return this.value;
    },
    function () {
        return String(this.value);
    }
);
const numberVariable = {"x": 0, "y": 1, "z": 2};
const Variable = function (name) {
    this.name = name;
}
newExpression(Variable,
    function (...func) {
        return func[numberVariable[this.name]];
    },
    function () {
        return this.name;
    }
);

function Operation(...args) {
    this.args = args;
}
newExpression(Operation,
    function (...values) {
        return this.calc(...this.args.map(el => el.evaluate(...values)));
    },
    function () {
        return this.args.map(e => e.toString()).join(' ') + ' ' + this.operator;
    }
);

function createOperation(operator, operation) {
    const op = function (...args) {
        Operation.call(this, ...args);
    }
    op.prototype = Object.create(Operation.prototype);
    op.prototype.operator = operator;
    op.prototype.calc = operation;
    //op.prototype.priority = priority;
    return op;
}

const Add = createOperation("+", (a, b) => a + b);
const Subtract = createOperation("-",(a, b) => a - b);
const Multiply = createOperation("*",(a, b) => a * b);
const Divide = createOperation("/",(a, b) => a / b);
const Negate = createOperation("negate",a => -1 * a);
const Exp = createOperation("exp", a => Math.exp(a));
const Ln = createOperation("ln", a => Math.log(a));

const mapOfOperations = {
    '+': Add,
    '-': Subtract,
    '*': Multiply,
    '/': Divide,
    'negate': Negate,
    'exp': Exp,
    'ln': Ln,
};

const mapOfSizes = {
    '+': 2,
    '-': 2,
    '*': 2,
    '/': 2,
    'negate': 1,
    'exp': 1,
    'ln': 1,
}

const parse = expr => {
    let stack = [];
    expr = expr.split(/\s+/);
    for (let i = 0; i < expr.length; ++i) {
        if (expr[i] in mapOfOperations) {
            let args = [];
            for (let j = 0; j < mapOfSizes[expr[i]]; j++) {
                args.push(stack.pop());
            }
            if (args.length === 2) {
                stack.push(new mapOfOperations[expr[i]](args[1], args[0]));
            } else if (args.length === 1) {
                stack.push(new mapOfOperations[expr[i]](args[0]));
            }
        } else {
            if (expr[i] in numberVariable) {
                stack.push(new Variable(expr[i]));
            } else if (expr[i] !== "") {
                stack.push(new Const(parseFloat(expr[i])));
            }
        }
    }
    return stack[0];
}