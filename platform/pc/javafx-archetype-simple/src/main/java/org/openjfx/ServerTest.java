package org.openjfx;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;

public class ServerTest {
    public static void main(String[] args) {
        new ServerTest().los();
    }
    public void los() {
        try (Socket sock = new Socket("192.168.178.200", 8088)) {
            PrintWriter writer = new PrintWriter(sock.getOutputStream());
            String antwort = sayHi();
            writer.println(antwort);
            writer.close();
            System.out.println("Client: " + antwort);
        } catch(IOException ex) {
            ex.printStackTrace();
        }
    }
    private String sayHi(){
        return "Hello World";
    }

}