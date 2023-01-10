package org.openjfx;

import java.io.IOException;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args ) throws IOException {
        svgExtragtMetadata svgExtragtMetadata = new svgExtragtMetadata();
        org.openjfx.svgExtragtMetadata.main();
        TxtToArduino translate = new TxtToArduino();
        translate.main();

        System.out.println( "finish" );
    }
}
