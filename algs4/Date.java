/**
 * The Date class is an immutable data type to encapsulate a date (day, month, and year).
 */
public class Date implements Comparable<Date> {
    private static final int[] DAYS = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    private final int month;    // month (between 1 and 12)
    private final int day;      // day (between 1, and DAYS[month]
    private final int year;     // year

    // Is y a leap(闰年) year?
    private static boolean isLeapYear(int y) {
        if (y % 400 == 0)
            return true;
        if (y % 100 == 0)
            return false;
        return y % 4 == 0;
    }

    // Is the given date valid?
    private static boolean isValid(int m, int d, int y) {
        if (m < 1 || m > 12)
            return false;
        if (d < 1 || d > DAYS[m])
            return false;
        return m != 2 || d != 29 || isLeapYear(y);
    }

    /**
     * Initialize a new date from the month, day, and year.
     *
     * @param month the month (between 1 and 12)
     * @param day   the day (between 1 and 28-31, depending on the month
     * @param year  the year
     */
    public Date(int month, int day, int year) {
        if (!isValid(month, day, year))
            throw new IllegalArgumentException("Invalid date");
        this.month = month;
        this.day = day;
        this.year = year;
    }

    /**
     * Initialize new date specified as a string in form MM/DD/YYYY.
     *
     * @param date the string representation of this date
     */
    public Date(String date) {
        String[] fields = date.split("/");
        if (fields.length != 3)
            throw new IllegalArgumentException("Invalid date");
        month = Integer.parseInt(fields[0]);
        day = Integer.parseInt(fields[1]);
        year = Integer.parseInt(fields[2]);
        if (!isValid(month, day, year))
            throw new IllegalArgumentException("Invalid date");
    }

    /**
     * Return the month.
     *
     * @return the month (an integer between 1 and 12)
     */
    public int month() {
        return month;
    }

    /**
     * Returns the day.
     *
     * @return the day (an integer between 1 and 31)
     */
    public int day() {
        return day;
    }

    /**
     * Returns the year.
     *
     * @return the year
     */
    public int year() {
        return year;
    }

    /**
     * Compares two dates chronologically.
     *
     * @return the value 0 if the argument date is equal to this date;
     * a negative integer if this date is chronologically less than the argument date;
     * and a positive integer if this date is chronologically after the argument date
     */
    @Override
    public int compareTo(Date that) {
        if (this.year < that.year)
            return -1;
        if (this.year > that.year)
            return +1;
        if (this.month < that.month)
            return -1;
        if (this.month > that.month)
            return +1;
        return Integer.compare(this.day, that.day);
    }

    /**
     * Return the next date in the calender.
     *
     * @return a date that represents the next day after this day
     */
    public Date next() {
        if (isValid(month, day + 1, year))
            return new Date(month, day + 1, year);
        else if (isValid(month + 1, 1, year))
            return new Date(month + 1, 1, year);
        else
            return new Date(1, 1, year + 1);
    }

    /**
     * Compares two dates chronologically.
     *
     * @param that the other date
     * @return true if this date is after that date; false otherwise
     */
    public boolean isAfter(Date that) {
        return this.compareTo(that) > 0;
    }

    /**
     * Compares two dates chronologically.
     *
     * @param that the other date
     * @return true if this date is before that date; false otherwise
     */
    public boolean isBefore(Date that) {
        return this.compareTo(that) < 0;
    }

    /**
     * Return a string representation of this date.
     *
     * @return the string representation in the format MM/DD/YYYY
     */
    @Override
    public String toString() {
        return month + "/" + day + "/" + year;
    }

    /**
     * Compares this date to the specified date.
     *
     * @param other the other date
     * @return true if this date equals other; false otherwise
     */
    @Override
    public boolean equals(Object other) {
        if (this == other) return true;
        if (other == null || getClass() != other.getClass()) return false;
        Date date = (Date) other;
        return month == date.month && day == date.day && year == date.year;
    }

    /**
     * Returns an integer hash code for this date
     *
     * @return an integer hash code for this date
     */
    @Override
    public int hashCode() {
        return day + 31 * month + 31 * 12 * year;
    }

    public static void main(String[] args) {
        Date today = new Date(2, 25, 2005);
        System.out.println("Today " + today);
        for (int i = 0; i < 10; i++)
            today = today.next();
        System.out.println("Ten days after is " + today);

        System.out.println("Today" + (today.isAfter(today.next()) ? " after " : " before ") + "tomorrow");
        System.out.println("Tomorrow" + (today.next().isBefore(today) ? " before " : " after ") + "today");

        Date copy = new Date(today.toString());
        System.out.println("Month " + copy.month() + " day " + copy.day() + " year " + copy.year());
    }
}
