import java.io.*; //DataOutputStream
import java.net.*; //Socket
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter; //time
import java.util.Scanner;

public class Client {

    private static Socket socket;
    private static Sender sender;
    private static Receiver receiver;
    private static String Ip;
    private static String name;
    s
    public static void main(String args[]) {
        if (args.length != 3) {
            System.out.println("./Client IP Port name");
            System.exit(0);
        }

        Ip = args[0];
        int Port = Integer.parseInt(args[1]);
        name = args[2];

        try {
            Socket socket = new Socket(Ip, Port);

            System.out.println("Server connect");
            Thread Sender = new Thread(new Sender(socket, name)); //send thread
            Thread Receiver = new Thread(new Receiver(socket));

            Sender.start();
            Receiver.start();
        }
        catch (ConnectException e1) {
            e1.printStackTrace();
        }
        catch (Exception e2) {}

    } // main

    static class Sender extends Thread { //보내는 함수
        Time times = new Time(); //시간
        Socket socket;
        PrintWriter out;
        String name;

        Sender(Socket socket, String name) {
            this.socket = socket;
            this.name = name;
            try {
                out = new PrintWriter(socket.getOutputStream(), true);
            }
            catch (Exception e) {}
        } //생성자

        public void run() {
            Scanner sc = new Scanner(System.in);
            String message = "";
            String check;
            try {
                if (out != null) {
                    out.println(name);
                    out.println(name + "이름으로 연결되었습니다.");
                }
                while (out != null) {
                    message = sc.nextLine();
                    if ("exit".equals(message)) {
                        sc.close();
                        socket.close();
                        out.close();
                        System.out.println("exit chating");
                        System.exit(0);
                    }
                    else if (".help".equalsIgnoreCase(message)) { //fix
                        System.out.println("exit : 채팅방 나가기");
                        System.out.println("/w name message : 귓속말");
                        System.out.println("/server create name : 채팅방 만들기");
                        System.out.println("/server join name : 채팅방 들어가기");
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
                            System.out.println("잘못된 명령어 입니다.\n /w name message");
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
                            System.out.println("잘못된 명령어 입니다.\n /server (create or join) name");
                        }
                    }
                    else if (message.startsWith("#PORT")) {
                        out.println(message);
                    }
                    else {
                        out.println(times.Gettime() + "[" + name + "] : " + message); //plus time
                    }
                }
            }
            catch (IOException e) {}
        } //run

        public void updateSocket(Socket newSocket) {
            this.socket = newSocket;
            try {
                out = new PrintWriter(newSocket.getOutputStream(), true);
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }//update

    } //sender class

    static class Receiver extends Thread { //받는 함수
        int Portnum;
        Socket socket;
        BufferedReader in;
        String[] command;
        String message;
        Receiver(Socket socket) {
            this.socket = socket;
            try {
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            }
            catch (IOException e) {}
        } //생성자

        public void run() {
            while (in != null) {
                try {
                    message = in.readLine();
                    if (message.startsWith("#PORT")) {
                        System.out.println("asdasdasdasd");
                        command = message.split(" ", 2);
                        JoinServer(command[1]);
                    }
                    else
                        System.out.println(message);
                }
                catch (IOException e) {}
            }
        } //run


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

    static public class Time { //시간 함수
        public String Gettime() {
            LocalDateTime now = LocalDateTime.now();
            DateTimeFormatter formats = DateTimeFormatter.ofPattern("HH:mm:ss");
            return now.format(formats);
        }
    }//time

    public static void JoinServer(String newPort) {
        try {
            if (socket != null) {
                socket.close();
            }

            socket = new Socket(Ip, Integer.parseInt(newPort));

            if (sender != null) {
                sender.interrupt(); // 안전하게 스레드 정지
            }
            if (receiver != null) {
                receiver.interrupt(); // 안전하게 스레드 정지
            }

            sender = new Sender(socket, name);   // 새 스레드 생성 및 시작
            receiver = new Receiver(socket);
            new Thread(sender).start();
            new Thread(receiver).start();

        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }//joinserver

} //main class
