import java.util.*; //scanner
import java.io.*; //DataOutputStream
import java.net.*; //Socket
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter; //time

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
            Thread Sender = new Thread(new Sender(socket, name)); //send thread
        }
        catch (ConnectException e1) {
            e1.printStackTrace();
        }
        catch (Exception e2) {}

    } // main

    static class Sender extends Thread {
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
        }
        public void run() {
            Scanner sc = new Scanner(System.in);
            String message = "";
            try {
                if (out != null) {
                    out.writeUTF(name + "connect");
                }
                if ("eixt".equals(message)) {
                    System.out.println("exit chating");
                    System.exit(0);
                }
                else if (".help".equals(message)) { //mmmmmmmmmmmmmmmmmmmmmmmmmmm
                    System.out.println("exit : ");
                }
                else {
                    out.writeUTF(time "[" + "]"); //plus time
                }

            }
            catch (IOException e) {}
            sc.close();

        } //»ý¼ºÀÚ
    } //sender class
} //main class



