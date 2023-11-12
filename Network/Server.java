 import java.io.*;
import java.net.*;
import java.util.concurrent.ConcurrentHashMap;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Serverf {
    final String EXIT = "exit";
    int num_user = 0;
    int num_chat = 0;
    final int portNumber = 9190;    // portNumber = 9190 ����� ����
    ConcurrentHashMap <Thread, ClientInfo> clientMap = new ConcurrentHashMap<>();
    public void start(){
        try {
            /*socket �Լ� ȣ��, bind �Լ� ȣ�� */   
            ServerSocket serverSocket = new ServerSocket(portNumber); // ���� ���� ����
            System.out.println("������ ���۵Ǿ����ϴ�. ��Ʈ ��ȣ: " + portNumber); // ���

            

            while (true) { // ���ѹݺ�
                /*listen �Լ� ȣ�� accept �Լ� ȣ��  */
                Socket clientSocket = serverSocket.accept();
                System.out.println("Ŭ���̾�Ʈ�� ����Ǿ����ϴ�.");
                ClientInfo clientInfo = new ClientInfo(clientSocket);

                Thread clientThread = new Thread(new ClientHandler(clientSocket, clientInfo, this));
                clientMap.put(clientThread, clientInfo);  // Thread�� KEY Socket�� VALUE
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

    
    /*���� �޽��� ������
    public void sendMessageToClient(Thread clientThread, String message) {
        ClientInfo clientInfo = clientMap.get(clientThread);  // �ش� �����忡 �´� ��ü�� ������
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
    } // ������


    @Override
    public void run() {
        try {
            
            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream())); // ����

            String message;
            String[] command;
            name = in.readLine();
            clientinfo.setName(name);
            
            while ((message = in.readLine()) != null) {
                if (message.charAt(0) == '/'){
                    // ��ɾ�
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
            try { // null�� ��� close�ϸ� NullPointerException������ �߻�, �̰��� ���� �ϴ� if��
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