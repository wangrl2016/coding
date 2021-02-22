public class Student {
    public int rollNum;
    public String name;

    public Student(int rollNum, String name) {
        this.rollNum = rollNum;
        this.name = name;
    }

    public static void main(String[] args) {
        // Declares an array of integers.
        Student[] s;
        // Allocating memory for 2 objects of type Student.
        s = new Student[2];
        // Initialize the elements.
        s[0] = new Student(1, "aman");
        s[1] = new Student(2, "vaibhav");

        for (int i = 0; i < s.length; i++)
            System.out.println("Element at " + i + ": " +
                    s[i].rollNum + " " + s[i].name);
    }
}
