import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        final int portNumber = 12345;
        
        try {
            ServerSocket serverSocket = new ServerSocket(portNumber);
            System.out.println("서버가 시작되었습니다. 포트 번호: " + portNumber);
            
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("클라이언트가 연결되었습니다.");
                
               
                Thread clientThread = new Thread(new ClientHandler(clientSocket));  // 클라이언트와 통신을 위한 스레드 시작
                clientThread.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

class ClientHandler implements Runnable {
    private Socket clientSocket;
    private PrintWriter out;
    private BufferedReader in;
    
    public ClientHandler(Socket socket) {
        this.clientSocket = socket;
    }
    
    @Override
    public void run() {
        try {
            out = new PrintWriter(clientSocket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            
            String inputLine;
            while ((inputLine = in.readLine()) != null) {
                System.out.println("클라이언트: " + inputLine);
                out.println("서버: " + inputLine);
            }
            
            in.close();
            out.close();
            clientSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
