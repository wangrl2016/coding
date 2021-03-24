import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdRandom;

import java.util.Comparator;

/**
 * The Point2D class is an immutable data type to encapsulate a
 * two-dimensional point with real-value coordinates.
 *
 * Note: in order to deal with the difference behavior of double and
 * Double with respect to -0.0 and +0.0, the Point2D constructor converts
 * any coordinates that are -0.0 to +0.0.
 */
public class Point2D implements Comparable<Point2D> {

    private final double x;     // x coordinate
    private final double y;     // y coordinate

    public Point2D(double x, double y) {
        if (Double.isInfinite(x) || Double.isInfinite(y))
            throw new IllegalArgumentException("Coordinates must be finite");
        if (Double.isNaN(x) || Double.isNaN(y))
            throw new IllegalArgumentException("Coordinate can't be NaN");
        if (x == 0.0)
            this.x = 0.0;       // convert -0.0 to +0.0
        else
            this.x = x;
        if (y == 0.0)
            this.y = 0.0;       // convert -0.0 to +0.0
        else
            this.y = y;
    }

    /**
     * Return the x-coordinate
     *
     * @return the x-coordinate
     */
    public double x() {
        return x;
    }

    /**
     * Return the y-coordinate
     *
     * @return the y-coordinate
     */
    public double y() {
        return y;
    }

    /**
     * Return the polar radius of this point.
     *
     * @return the polar radius of this point in polar coordinates: sqrt(x*x +y*y)
     */
    public double r() {
        return Math.sqrt(x * x + y * y);
    }

    /**
     * Returns the angle of this point in polar coordinates.
     *
     * @return the angle (in radians) of this point in polar coordinates (between -pi and pi)
     */
    public double theta() {
        return Math.atan2(y, x);
    }

    /**
     * Return the angle between this point and that point.
     *
     * @return the angle in radians (between -pi and pi) between this point and that point (0 if equal)
     */
    private double angleTo(Point2D that) {
        double dx = that.x - this.x;
        double dy = that.y - this.y;
        return Math.atan2(dy, dx);
    }

    /**
     * 由三角形顶点求三角形面积参考链接
     *
     * https://blog.csdn.net/chenyiming_1990/article/details/8838917
     *
     * Return the signed area of the triangle a-b-c.
     *
     * @param a first point
     * @param b second point
     * @param c third point
     * @return the signed area of the triangle a-b-c
     */
    public static double area(Point2D a, Point2D b, Point2D c) {
        return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) / 2.0;
    }

    /**
     * 判断三角形是否逆时针
     * Return true if a -> b -> c is counter clockwise turn.
     *
     * @param a first point
     * @param b second point
     * @param c third point
     * @return {-1, 0, +1} if a -> b -> c is a {clockwise, collinear, counter clockwise} turn
     */
    public static int counterClockwise(Point2D a, Point2D b, Point2D c) {
        double area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        if (area < 0)
            return -1;
        else if (area > 0)
            return 1;
        else
            return 0;
    }

    /**
     * Returns the euclidean distance between this point and that point.
     *
     * @param that the other point
     * @return the euclidean distance between this point and that point
     */
    public double distanceTo(Point2D that) {
        double dx = this.x - that.x;
        double dy = this.y - that.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    /**
     * Returns the squares of the euclidean distance between this point and that point.
     *
     * @param that the other point
     * @return the square of the euclidean distance between this point and that point
     */
    public double distanceSquaredTo(Point2D that) {
        double dx = this.x - that.x;
        double dy = this.y - that.y;
        return dx * dx + dy * dy;
    }

    /**
     * Compares two points by y-coordinate, breaking ties by x-coordinate.
     * Formally, the invoking point (x0, y0) is less than the argument point (x1, y1)
     * if and only if either (y0 < y1) or (if y0 == y1 and x0 < x1).
     *
     * @param that the other point
     * @return the value 0 if this string is equal to the argument string returns;
     * a negative integer if thia point is less than the argument point;
     * and positive integer if this point is greater than the argument point
     */
    @Override
    public int compareTo(Point2D that) {
        if (this.y < that.y)
            return -1;
        if (this.y > that.y)
            return +1;
        return Double.compare(this.x, that.x);
    }

    // Compare points according to their x-coordinate.
    private static class XOrder implements Comparator<Point2D> {
        @Override
        public int compare(Point2D p, Point2D q) {
            return Double.compare(p.x, q.x);
        }
    }

    // Compare points according to their y-coordinate.
    private static class YOrder implements Comparator<Point2D> {
        @Override
        public int compare(Point2D p, Point2D q) {
            return Double.compare(p.y, q.y);
        }
    }

    // Compare points according to their polar radius.
    private static class ROrder implements Comparator<Point2D> {
        @Override
        public int compare(Point2D p, Point2D q) {
            double delta = (p.x * p.x + p.y * p.y) - (q.x * q.x + q.y * q.y);
            if (delta < 0)
                return -1;
            else if (delta > 0)
                return +1;
            else
                return 0;
        }
    }

    // Compare other points relative to atan2 angle (between -pi/2 and pi/2) they make with this Point.
    private class Atan2Order implements Comparator<Point2D> {
        @Override
        public int compare(Point2D p, Point2D q) {
            double angle1 = angleTo(p);
            double angle2 = angleTo(q);
            return Double.compare(angle1, angle2);
        }
    }

    // Compare other points relative to polar angle (between 0 and 2pi) they make with this Point.
    private class PolarOrder implements Comparator<Point2D> {
        @Override
        public int compare(Point2D p, Point2D q) {
            double dx1 = p.x - x;
            double dy1 = p.y - y;
            double dx2 = q.x - x;
            double dy2 = q.y - y;
            if (dy1 >= 0 && dy2 < 0)
                return -1;  // p above; q below
            else if (dy2 >= 0 && dy1 < 0)
                return +1; // p below; q above
            else if (dy1 == 0 && dy2 == 0) {
                if (dx1 >= 0 && dx2 < 0)
                    return -1;
                else if (dx2 >= 0 && dx1 < 0)
                    return +1;
                else
                    return 0;
            } else
                return -counterClockwise(Point2D.this, p, q);
        }
    }

    // Compare points according to their distance to this point.
    private class DistanceToOrder implements Comparator<Point2D> {
        @Override
        public int compare(Point2D p, Point2D q) {
            double dist1 = distanceSquaredTo(p);
            double dist2 = distanceSquaredTo(q);
            return Double.compare(dist1, dist2);
        }
    }

    /**
     * Compare this point to the specified point.
     *
     * @param other the other point
     * @return true if this point equals other; false otherwise
     */
    @Override
    public boolean equals(Object other) {
        if (this == other) return true;
        if (other == null || getClass() != other.getClass()) return false;
        Point2D that = (Point2D) other;
        return Double.compare(that.x, x) == 0 &&
                Double.compare(that.y, y) == 0;
    }

    /**
     * Returns a string representation of this point.
     *
     * @return a string representation of this point in the format (x, y)
     */
    @Override
    public String toString() {
        return "(" + x + ", " + y + ")";
    }

    /**
     * Plot this point using standard draw.
     */
    public void draw() {
        StdDraw.point(x, y);
    }

    /**
     * Plot a line from this point to that point using standard draw.
     *
     * @param that the other point
     */
    public void drawTo(Point2D that) {
        StdDraw.line(this.x, this.y, that.x, that.y);
    }

    @Override
    public int hashCode() {
        int hashX = ((Double) x).hashCode();
        int hashY = ((Double) y).hashCode();
        return 31 * hashX + hashY;
    }

    public static void main(String[] args) {
        if (args.length < 6)
            System.exit(1);
        Point2D p1 = new Point2D(Double.parseDouble(args[0]), Double.parseDouble(args[1]));
        Point2D p2 = new Point2D(Double.parseDouble(args[2]), Double.parseDouble(args[3]));
        Point2D p3 = new Point2D(Double.parseDouble(args[4]), Double.parseDouble(args[5]));
        // 默认(512, 512)大小
        int DEFAULT_WIDTH = 512;
        int DEFAULT_HEIGHT = 512;
        StdDraw.setXscale(0, DEFAULT_WIDTH);
        StdDraw.setYscale(0, DEFAULT_HEIGHT);
        StdDraw.setPenRadius(0.05);
        StdDraw.setPenColor(StdDraw.RED);
        StdDraw.enableDoubleBuffering();

        // 绘制三个点
        p1.draw();
        p2.draw();
        p3.draw();

        // 连接三条线
        StdDraw.setPenRadius(0.01);
        p1.drawTo(p2);
        p2.drawTo(p3);
        p3.drawTo(p1);

        StdDraw.show();
        StdDraw.save("out/point-2d.png");
        System.exit(0);
    }
}
