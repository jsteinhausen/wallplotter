package org.openjfx;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;

public class ServerTest {
    public static void main(String[] args) throws IOException {
        new ServerTest().los();
    }
    public void los() throws IOException {
        WallplotterServer wallplotterServer=new WallplotterServer();
        wallplotterServer.init();
        PrintWriter writer = new PrintWriter(wallplotterServer.getSocket().getOutputStream());
        String antwort = sayHi();
        writer.println(antwort);
        writer.close();
        System.out.println("Client: " + antwort);
    }
    private String sayHi(){
        return "Hello World";
    }

}