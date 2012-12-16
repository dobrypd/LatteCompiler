import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Runtime {
    public static BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

    public static void printInt(int x)
    {
        System.out.println(x);
    }
    public static void printString(String x)
    {
        System.out.println(x);
    }
    public static void error()
    {
        System.out.println("runtime error");
        System.exit(1);
    }
    public static int readInt()
    {
        try {
            return Integer.parseInt(br.readLine());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }
    public static String readString()
    {
        try {
            return br.readLine();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }
}
