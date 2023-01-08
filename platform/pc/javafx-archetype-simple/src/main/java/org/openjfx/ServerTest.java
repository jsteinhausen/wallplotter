package org.openjfx;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;

public class ServerTest {
    String[] myCommands={"move_pen_abs(12147.04,7806.9);", "draw_line(0,0,0,-3908.4);", "draw_line(0,0,518.57,0);", "draw_line(0,0,0,1610.3);", "draw_line(0,0,2019.7,0);", "draw_line(0,0,0,-1610.3);}","endfile();"};

    public static void main(String[] args) throws IOException {
        new ServerTest().los();
    }
    public void los() throws IOException {
        WallplotterServer wallplotterServer=new WallplotterServer();
        Wallplotter wallplotter= new Wallplotter(wallplotterServer);
        /*for(int i=0; i<myCommands.length;i++){
            wallplotter.println(myCommands[0]);
        }*/
        wallplotter.println(sayHi());
        System.out.println("Client: " + sayHi());
        wallplotter.println(ask());
        System.out.println("Client: " + ask());
        wallplotter.println(askweaher());
        System.out.println("Client: " + askweaher());
        wallplotterServer.getSocket().close();
    }
    private String sayHi(){
        return "Hello World";
    }
    private String ask(){
        return "How are you?";
    }
    private String askweaher(){
        return "How is the weather today?";
    }

}