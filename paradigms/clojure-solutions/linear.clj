(defn makeOperation[op] (fn[a b] (mapv op a b)))
(def v+ (makeOperation +))
(def v- (makeOperation -))
(def v* (makeOperation *))
(def vd (makeOperation /))
(def m+ (makeOperation v+))
(def m- (makeOperation v-))
(def m* (makeOperation v*))
(def md (makeOperation vd))

(defn scalar ([a b] (apply + (v* a b))))
(defn m*v [m v] (mapv #(scalar % v) m))

(defn vect [v w]
  (vector (- (* (v 1) (w 2)) (* (v 2) (w 1)))           ;v_y * w_z - v_z * w_y
          (- (* (v 2) (w 0)) (* (v 0) (w 2)))           ;v_z * w_x - v_x * w_z
          (- (* (v 0) (w 1)) (* (v 1) (w 0)))))         ;v_x * w_y - v_y * w_x

(defn v*s [v s] (mapv #(* s %) v))
(defn m*s [m s] (mapv #(v*s % s) m))

(defn transpose [m] (apply mapv vector m))
(defn m*m [m1 m2] (transpose (mapv #(m*v m1 %) (transpose m2))))

(defn recursiveDescent [op] (fn [a b]
    (if (vector? a)
      (mapv (recursiveDescent op) a b)
      (op a b))))

(def s+ (recursiveDescent +))
(def s- (recursiveDescent -))
(def s* (recursiveDescent *))
(def sd (recursiveDescent /))
