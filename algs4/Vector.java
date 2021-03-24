import java.text.DecimalFormat;

/**
 * 不可变向量实现
 *
 * Implementation of a vector of real numbers.
 *
 * This class is implemented to be immutable: once the client program
 * initialize a Vector, it cannot change any of its fields (d or data[i])
 * either directly or indirectly.
 * Immutability is a very desirable feature of a data type.
 *
 * The Vector class represents a d-dimensional Euclidean vector.
 * Vectors are immutable: their values cannot be changed after they are created.
 *
 * It includes methods for addition, subtraction, dot product, scalar product,
 * unit vector, Euclidean norm, and the Euclidean distance between two vectors.
 */
public class Vector {
    private int d;
    private double[] data;

    /**
     * Initializes a d-dimensional zero vector.
     *
     * @param d the dimension of the vector
     */
    public Vector(int d) {
        this.d = d;
        data = new double[d];
    }

    /**
     * Initializes a vector from either an array or a vararg list.
     * The vararg syntax supports a constructor that takes a variable number of
     * arguments such as Vector x = new Vector(1.0, 2.0, 3.0, 4.0).
     *
     * @param a the array or vararg list
     */
    public Vector(double... a) {
        d = a.length;

        // Defensive(防御性的) copy so that client can't alter our copy of data[].
        data = new double[d];
        System.arraycopy(a, 0, data, 0, d);
    }

    /**
     * Returns the dimension of this vector.
     *
     * @return the dimension of this vector.
     */
    public int dimension() {
        return d;
    }

    /**
     * Returns the dot product of this vector with the specified vector.
     * a = {a1, a2, a3}, b = {b1, b2, b3}
     * dot = a1 * b1 + a2 * b2 + a3 *b3
     *
     * @param that the other vector
     * @return the dot product of this vector and that vector.
     */
    public double dot(Vector that) {
        if (this.d != that.d)
            throw new IllegalArgumentException("Dimension don't agree");
        double sum = 0.0;
        for (int i = 0; i < d; i++)
            sum = sum + (this.data[i] * this.data[i]);
        return sum;
    }

    /**
     * Return the magnitude(大小) of this vector.
     * This also known as the L2 norm or the Euclidean norm.
     *
     * @return the magnitude of this vector
     */
    public double magnitude() {
        return Math.sqrt(this.dot(this));
    }

    /**
     * Returns the sum of this vector and the specified vector.
     *
     * @param that the vector to add to this vector
     * @return the vector whose value is (this +that)
     */
    public Vector plus(Vector that) {
        if (this.d != that.d)
            throw new IllegalArgumentException("Dimensions don't agree");
        Vector c = new Vector(d);
        for (int i = 0; i < d; i++)
            c.data[i] = this.data[i] + that.data[i];
        return c;
    }

    /**
     * Returns the difference between this vector and the specified vector.
     *
     * @param that the vector to subtract from this vector
     * @return the vector whose value is (this - that)
     */
    public Vector minus(Vector that) {
        if (this.d != that.d)
            throw new IllegalArgumentException("Dimension don't agree");
        Vector c = new Vector(d);
        for (int i = 0; i < d; i++)
            c.data[i] = this.data[i] - that.data[i];
        return c;
    }

    /**
     * Return that Euclidean distance between this vector and the specified vector.
     *
     * @param that the other vector
     * @return the Euclidean distance between this vector and that vector
     */
    public double distanceTo(Vector that) {
        if (this.d != that.d)
            throw new IllegalArgumentException("Dimension don't agree");
        return this.minus(that).magnitude();
    }

    /**
     * Returns the ith cartesian coordinate.
     *
     * @param i the coordinate index
     * @return the ith cartesian coordinate
     */
    public double cartesian(int i) {
        if (i >= d)
            throw new IllegalArgumentException("Dimensions overflow");
        return data[i];
    }

    /**
     * Returns the scalar-vector product of this vector and the specified scalar.
     *
     * @param alpha the scalar
     * @return the vector whose value is alpha * this
     */
    public Vector scale(double alpha, int dot) {
        StringBuilder sb = new StringBuilder("0.");
        sb.append("0".repeat(Math.max(0, dot)));
        DecimalFormat df = new DecimalFormat(sb.toString());
        Vector c = new Vector(d);
        for (int i = 0; i < d; i++)
            c.data[i] = Double.parseDouble(df.format(alpha * data[i]));
        return c;
    }

    /**
     * Returns a unit vector in the direction of this vector.
     *
     * @return a unit vector in the direction of this vector
     */
    public Vector direction() {
        if (this.magnitude() == 0.0)
            throw new ArithmeticException("Zero vector has no direction");
        return this.scale(1.0 / this.magnitude(), 2);
    }

    @Override
    public String toString() {
        StringBuilder s = new StringBuilder();
        for (int i = 0; i < d; i++)
            s.append(data[i]).append(" ");
        return s.toString();
    }

    /**
     * Unit tests the Vector data type.
     */
    public static void main(String[] args) {
        DecimalFormat df = new DecimalFormat("0.00");
        double[] vector1 = {1.0, 2.0, 3.0, 4.0};
        double[] vector2 = {1.0, 2.0, 3.0, 4.0};
        Vector x = new Vector(vector1);
        Vector y = new Vector(vector2);
        System.out.println("x = " + x);
        System.out.println("y = " + y);
        System.out.println("x + y = " + x.plus(y));
        System.out.println("x - y = " + x.minus(y));
        System.out.println("10x = " + x.scale(10.0, 1));
        System.out.println("|x| = " + df.format(x.magnitude()));
        System.out.println("<x, y> = " + x.dot(y));
        System.out.println("dist(x, y) = " + x.distanceTo(y));
        System.out.println("dir(x) = " + x.direction());
        System.out.println("dimension(x) =" + x.dimension());
        System.out.println("x[2] = " + x.cartesian(2));
    }
}
