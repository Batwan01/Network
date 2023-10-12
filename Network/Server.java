import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        final int portNumber = 12345;
        
        try {
            ServerSocket serverSocket = new ServerSocket(portNumber);
            System.out.println("������ ���۵Ǿ����ϴ�. ��Ʈ ��ȣ: " + portNumber);
            
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Ŭ���̾�Ʈ�� ����Ǿ����ϴ�.");
                
               
                Thread clientThread = new Thread(new ClientHandler(clientSocket));  // Ŭ���̾�Ʈ�� ����� ���� ������ ����
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
                System.out.println("Ŭ���̾�Ʈ: " + inputLine);
                out.println("����: " + inputLine);
            }
            
            in.close();
            out.close();
            clientSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
