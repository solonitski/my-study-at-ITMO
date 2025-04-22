"use strict";

//
const unaryOperation = operation => (func) => (...args) => operation(func(...args));
const cnst = a => () => a;
const negate = unaryOperation(a => -a);
const sin = unaryOperation(Math.sin);
const cos = unaryOperation(a => Math.cos(a));
const one = cnst(1);
const two = cnst(2);
const binaryOperation = operation => (...func) => (...args) => operation(func[0](...args), func[1](...args));
const add = binaryOperation((x, y) => x + y);
const subtract = binaryOperation((x, y) => x - y);
const multiply = binaryOperation((x, y) => x * y);
const divide = binaryOperation((x, y) => x / y);
const numberVariable = {"x": 0, "y": 1, "z": 2};
const variable = name => (...func) => func[numberVariable[name]];

// ternaryOperation

let test = add(
    subtract(multiply(variable("x"), variable("x")), multiply(cnst(2), variable("x"))),
    cnst(1));
for (let i = 0; i <= 10; i++) {
    console.log(test(i));
}
