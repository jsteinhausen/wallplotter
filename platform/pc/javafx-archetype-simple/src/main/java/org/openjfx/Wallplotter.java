package org.openjfx;

import java.io.IOException;
import java.io.PrintWriter;

public class Wallplotter {
    WallplotterServer server;
    private final char END_OF_MESSAGE='~';

    public Wallplotter() throws IOException {
        this.server=new WallplotterServer();
    }

    public Wallplotter(String host, int port) throws IOException {
        this.server=new WallplotterServer(host,port);
    }
    public Wallplotter(WallplotterServer server) {
        this.server = server;
    }
    public void println(String message) throws IOException {
        PrintWriter writer = new PrintWriter(this.server.getSocket().getOutputStream());
        writer.println(message+END_OF_MESSAGE);
        writer.flush();
    }

    public WallplotterServer getServer() {
        return server;
    }

    public void setServer(WallplotterServer server) {
        this.server = server;
    }
}
