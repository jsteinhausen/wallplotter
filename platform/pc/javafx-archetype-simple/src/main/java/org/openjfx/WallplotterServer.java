package org.openjfx;

import java.io.IOException;
import java.net.Socket;

public class WallplotterServer {
    private String host;
    private int port;
    private Socket socket;

    public WallplotterServer(String host, int port) throws IOException {
        this.host = host;
        this.port = port;
        this.socket = new Socket(this.host, this.port);
    }

    public WallplotterServer() throws IOException {
        this.host = "192.168.4.1";
        this.port = 8088;
        this.socket = new Socket(this.host, this.port);
    }

    public boolean init() throws IOException {
       this.socket = new Socket(this.host, this.port);
            return true;
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
