;; HW10

(defn makeOp [operation]
  (fn [& args] (fn [vars] (apply operation (mapv #(% vars) args)))))

(def add (makeOp +))
(def multiply (makeOp *))
(def subtract (makeOp -))
(def negate subtract)
(def divide (makeOp (fn [a b] (/ (double a) (double b)))))
(def exp (makeOp #(. Math exp %)))
(def ln (makeOp #(. Math log %)))

(defn constant [val] (fn [_] val))
(defn variable [xyz] (fn [arg] (get arg xyz)))

(def mapOfOperations {'+      add
                      '-      subtract
                      '*      multiply
                      '/      divide
                      'negate negate
                      'exp    exp
                      'ln     ln
                      })

(defn parse [expr Map cnst vari]
  (cond (symbol? expr) (vari (str expr))
        (number? expr) (cnst expr)
        (list? expr) (apply (Map (first expr)) (mapv #(parse % Map cnst vari) (rest expr)))
        ))

(defn parseFunction
  [expr]
  (parse (read-string expr) mapOfOperations constant variable))

;; HW11
(defn evaluate [arg, args]
  ((:evaluate arg) args))

(defn toString [arg]
  (:toString arg))

(defn Constant [value]
  {
   :evaluate (fn [_] value)
   :toString (str value)
   })

(defn Variable [xyz]
  {
   :evaluate (fn [args] (args xyz))
   :toString (str xyz)
   })

(defn Operation [operator operation]
  (fn [& args]
    (let [evaluate-fn (fn [_args] (apply operation (map #((:evaluate %) _args) args)))
          toString-fn (str "(" operator " " (clojure.string/join " " (map #(:toString %) args)) ")")]
      {:evaluate evaluate-fn
       :toString toString-fn
       }
      )))

(def Add (Operation "+" +))
(def Subtract (Operation "-" -))
(def Multiply (Operation "*" *))
(def Divide (Operation "/" (fn [a b] (/ (double a) (double b)))))
(def Negate (Operation "negate" -))
(def Sin (Operation "sin" #(Math/sin %)))
(def Cos (Operation "cos" #(Math/cos %)))

(def mapOb {'+ Add,
            '- Subtract,
            '* Multiply,
            '/ Divide,
            'negate Negate,
            'sin Sin,
            'cos Cos})

(defn parseObject
  [expr]
  (parse (read-string expr) mapOb Constant Variable))