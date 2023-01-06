package org.openjfx;

import java.io.IOException;
import java.net.Socket;

public class WallplotterServer {
    private String host;
    private int port;
    private Socket socket;

    public WallplotterServer(String host, int port) {
        this.host = host;
        this.port = port;
    }

    public WallplotterServer() {
        this.host = "192.168.4.1";
        this.port = 8088;
    }

    public boolean init(){
        try (Socket tempSock = new Socket(this.host, this.port)) {
            this.socket=tempSock;
            return true;
        } catch(IOException ex) {
            ex.printStackTrace();
            return false;
        }
    }

    public String getHost() {
        return host;
    }

    public void setHost(String host) {
        this.host = host;
    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public Socket getSocket() {
        return this.socket;
    }

    public void setSocket(Socket socket) {
        this.socket = socket;
    }
}
