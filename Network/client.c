import java.io.*; //DataOutputStream
import java.net.*; //Socket
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter; //time
import java.util.Scanner;

public class Client {

    private static Socket socket;
    private static Thread Sender;
    private static Thread Receiver;
    private static String Ip;
    private static String name;

    public static void main(String args[]) {
        if (args.length != 3) {
            System.out.println("./Client IP Port name");
            System.exit(0);
        }

        Ip = args[0];
        int Port = Integer.parseInt(args[1]);
        name = args[2];

        try {
            socket = new Socket(Ip, Port);

            System.out.println("Server connect");
            Sender = new Thread(new Sender(socket, name)); //send thread
            Receiver = new Thread(new Receiver(socket));

            Sender.start();
            Receiver.start();
        }
        catch (ConnectException e1) {
            e1.printStackTrace();
        }
        catch (Exception e2) {}

    } // main

    static class Sender extends Thread { //������ �Լ�
        Time times = new Time(); //�ð�
        Socket socket;
        PrintWriter out;
        String name;
        Scanner sc = new Scanner(System.in);

        Sender(Socket socket, String name) {
            this.socket = socket;
            this.name = name;
            try {
                out = new PrintWriter(socket.getOutputStream(), true);
            }
            catch (Exception e) {}
        } //������

        public void run() {
            String message = "";
            String check;
            try {
                if (out != null) {
                    out.println(name);
                    out.println(name + "�̸����� ����Ǿ����ϴ�.");
                }
                while (out != null) {

                    message = sc.nextLine();
                    if ("/server list".equalsIgnoreCase(message)) {
                        out.println(message); // ������ ��ɾ� ����
                        continue; // ���� �Է��� ��ٸ�
                    }
                    if ("exit".equals(message)) {
                        sc.close();
                        socket.close();
                        out.close();
                        System.out.println("exit chating");
                        System.exit(0);
                    }
                    else if (".help".equalsIgnoreCase(message)) { //fix
                        System.out.println("exit : ä�ù� ������");
                        System.out.println("/w name message : �ӼӸ�");
                        System.out.println("/server create name : ä�ù� �����");
                        System.out.println("/server join name : ä�ù� ����");
                        System.out.println("/sever list : ä�ù� ����Ʈ����");
                    }
                    else if (message.startsWith("/w")) {
                        check = message;
                        int spaceCount = 0;
                        for (int i = 0; i < check.length(); i++) {
                            if (check.charAt(i) == ' ') {
                                spaceCount++;
                            }
                        }
                        if (spaceCount >= 2) {
                            out.println(message);
                        }
                        else {
                            System.out.println("�߸��� ��ɾ� �Դϴ�.\n /w name message");
                        }
                    }
                    else if (message.startsWith("/server")) {
                        check = message;
                        int spaceCount = 0;
                        for (int i = 0; i < check.length(); i++) {
                            if (check.charAt(i) == ' ') {
                                spaceCount++;
                            }
                        }
                        if (spaceCount >= 2) {
                            out.println(message);
                        }
                        else {
                            System.out.println("�߸��� ��ɾ� �Դϴ�.\n /server (create or join) name");
                        }
                    }
                    else {
                        out.println(times.Gettime() + "[" + name + "] : " + message); //plus time
                    }
                }
            }
            catch (IOException e) {}
        } //run

        public void close() {
            try {
                if (out != null) {
                    out.close();
                }
                if (socket != null) {
                    socket.close();
                }
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void updateSocket(Socket newSocket) {
            this.socket = newSocket;
            try {
                out = new PrintWriter(newSocket.getOutputStream(), true);
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }//update

    } //Sender class

    static class Receiver extends Thread { //�޴� �Լ�
        int Portnum;
        Socket socket;
        BufferedReader in;
        String[] command;
        String message;
        String check;
        Receiver(Socket socket) {
            this.socket = socket;
            try {
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            }
            catch (IOException e) {}
        } //������

        public void run() {
            while (in != null) {
                try {
                    message = in.readLine();
                    if (message == null) {
                        continue;
                    }
                    if (message.startsWith("#PORT")) {
                        command = message.split(" ", 2);
                        JoinServer(command[1]);
                    }
                    else
                        System.out.println(message);
                }
                catch (IOException e) {}
            }
        } //run

        public void close() {
            try {
                if (in != null) {
                    in.close();
                }
                if (socket != null) {
                    socket.close();
                }
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void updateSocket(Socket newSocket) {
            this.socket = newSocket;
            try {
                in = new BufferedReader(new InputStreamReader(newSocket.getInputStream()));
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }//update

    } //Receive

    static public class Time { //�ð� �Լ�
        public String Gettime() {
            LocalDateTime now = LocalDateTime.now();
            DateTimeFormatter formats = DateTimeFormatter.ofPattern("HH:mm:ss");
            return now.format(formats);
        }
    }//time

    public static void JoinServer(String newPort) {
        try {
            closeall();

            socket = new Socket(Ip, Integer.parseInt(newPort));
            Sender = new Sender(socket, name);   // �� ������ ���� �� ����
            Receiver = new Receiver(socket);

            new Thread(Sender).start();
            new Thread(Receiver).start();

        }
        catch (IOException e) {
            e.printStackTrace();
        }
        catch (InterruptedException e) {
        }
    }//joinserver

    private static void closeall() throws InterruptedException, IOException{
      if (Sender != null) {
          Sender.interrupt();
          Sender.join();
          Sender = null;
      }
      if (Receiver != null) {
          Receiver.interrupt();
          Receiver.join();
          Receiver = null;
      }
      if (socket != null && !socket.isClosed()) {
          socket.close();
      }
    } //close all

} //main class