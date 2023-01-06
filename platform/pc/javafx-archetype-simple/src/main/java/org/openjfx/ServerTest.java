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
        Wallplotter wallplotter= new Wallplotter(wallplotterServer);
        wallplotter.println(sayHi());
        System.out.println("Client: " + sayHi());
    }
    private String sayHi(){
        return "Hello World";
    }
    private String ask(){
        return "How are you?";
    }

}