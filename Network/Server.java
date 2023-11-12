 import java.io.*;
import java.net.*;
import java.util.concurrent.ConcurrentHashMap;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Serverf {
    final String EXIT = "exit";
    int num_user = 0;
    int num_chat = 0;
    final int portNumber = 9190;    // portNumber = 9190 상수로 선언
    ConcurrentHashMap <Thread, ClientInfo> clientMap = new ConcurrentHashMap<>();
    public void start(){
        try {
            /*socket 함수 호출, bind 함수 호출 */   
            ServerSocket serverSocket = new ServerSocket(portNumber); // 서버 소켓 생성
            System.out.println("서버가 시작되었습니다. 포트 번호: " + portNumber); // 출력

            

            while (true) { // 무한반복
                /*listen 함수 호출 accept 함수 호출  */
                Socket clientSocket = serverSocket.accept();
                System.out.println("클라이언트가 연결되었습니다.");
                ClientInfo clientInfo = new ClientInfo(clientSocket);

                Thread clientThread = new Thread(new ClientHandler(clientSocket, clientInfo, this));
                clientMap.put(clientThread, clientInfo);  // Thread가 KEY Socket이 VALUE
                clientThread.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    } // start

    public void removeClient(Thread clientThread) {
        clientMap.remove(clientThread);
    } // removeClient

    public void sendMessageToAll(String message){
        for(Thread clientThread : clientMap.keySet()){
            ClientInfo clientinfo = clientMap.get(clientThread);
            if(!clientinfo.getSocket().isClosed()){
                clientinfo.getOut().println(message);
            }
            else {
                removeClient(clientThread);
            }

        }
    } // sendMessageToAll

    
    /*개별 메시지 보내기
    public void sendMessageToClient(Thread clientThread, String message) {
        ClientInfo clientInfo = clientMap.get(clientThread);  // 해당 쓰레드에 맞는 객체를 가져옴
        out.println(now.format(formats) + name + " : " + inputLine);
        
    }
    */
    public void whisper(String receiver){
        
    }
    public static void main(String[] args) {
        new Serverf().start();        
    } // main
} // Serverf

class ClientHandler implements Runnable {
    private Serverf serverf;
    private String name = null;
    private Socket clientSocket;
    private ClientInfo clientinfo;
    private BufferedReader in;
    LocalDateTime now;

    public ClientHandler(Socket socket, ClientInfo clientinfo, Serverf serverf) {
        this.clientSocket = socket; this.clientinfo = clientinfo; this.serverf = serverf;
    } // 생성자


    @Override
    public void run() {
        try {
            
            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream())); // 받음

            String message;
            String[] command;
            name = in.readLine();
            clientinfo.setName(name);
            
            while ((message = in.readLine()) != null) {
                if (message.charAt(0) == '/'){
                    // 명령어
                    command = message.split(" ", 3);
                    
                    if (command[0].equals("/w")){
                        
                    }
                }
                else {
                    System.out.println(message);
                serverf.sendMessageToAll(message + "\n" + name);
                }
            }


        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try { // null인 경우 close하면 NullPointerException에러가 발생, 이것을 방지 하는 if문
                if (in != null) in.close();
                if (clientinfo.getOut() != null) clientinfo.getOut().close();
                if (clientSocket != null) clientSocket.close();
            } catch (IOException e) {
            e.printStackTrace();
            }
            serverf.removeClient(Thread.currentThread());
        }
    } // run
} // ClientHandler

class ClientInfo{
    private PrintWriter Out; 
    private String name;
    private Socket clientSocket;
    public Socket getSocket() {return clientSocket;}
    public String getName() {return name;}
    public PrintWriter getOut() {return Out;}
    public void setName(String name) {this.name = name;}
    public ClientInfo(Socket socket) throws IOException {
        this.clientSocket = socket;
        Out = new PrintWriter(clientSocket.getOutputStream(), true);
    }
    
}