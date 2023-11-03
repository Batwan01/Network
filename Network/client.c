import java.util.*; //scanner
import java.io.*; //DataOutputStream
import java.net.*; //Socket


public class Client {
    public static void main(String args[]) {
        if (args.length != 3) {
            System.out.println("./Client IP Port name");
            System.exit(0);
        }
        String Ip = args[0];
        int Port = Integer.parseInt(args[1]);
        String name = args[2];

        try {
            Socket socket = new Socket(Ip, Port);

            System.out.println("Server connect");
            // Thread Sender = new Thread(new Sender(socket,name)); 보내기+스레드 함수
        }
        catch (ConnectException e1) {
            e1.printStackTrace();
        }
        catch (Exception e2) {}

    } // main


    //작성중
    class Sender extends Thread { //static?
        Socket socket;
        DataOutputStream out;
        String name;

        Sender(Socket socket, String name) {
            this.socket = socket;
            try {
                out = new DataOutputStream(socket.getOutputStream());
                this.name = name;
            }
            catch (Exception e) {}

            //이제 run()으로 보내기 해야함

        } //생성자
    } //sender class
} //main class



