package org.openjfx;

import java.io.*;

public class svgExtragtMetadata {
    public static void main() {
        try {
            // Ouvre le fichier SGV en entrée
            BufferedReader in = new BufferedReader(new FileReader("src/main/java/org/openjfx/test.svg"));

            // Crée le fichier .txt en sortie
            BufferedWriter out = new BufferedWriter(new FileWriter("output.txt"));

            String line;
            boolean inMetadata = false;

            // Lit chaque ligne du fichier SGV en entrée
            while ((line = in.readLine()) != null) {
                // Si la ligne contient l'ouverture des métadonnées, on commence à copier
                if (line.contains("<metadata id=\"CorelCorpID_0Corel-Layer\"/>")) {
                    inMetadata = true;
                }

                // Si on est dans les métadonnées, on copie la ligne dans le fichier .txt
                if (inMetadata) {
                    out.write(line);
                    out.newLine();
                }

                // Si la ligne contient la fermeture des métadonnées, on arrête de copier
                if (line.contains("</g>")) {
                    inMetadata = false;
                    break;
                }
            }

            // Ferme les fichiers en entrée et en sortie
            in.close();
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            removeFirstAndLastLine("output.txt");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public static void removeFirstAndLastLine(String fileName) throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(fileName));
        String firstLine = reader.readLine(); // Première ligne
        String line;
        String lastLine = ""; // Dernière ligne
        StringBuilder inputBuffer = new StringBuilder();

        // Skip la première ligne
        while ((line = reader.readLine()) != null) {

            lastLine = line;
            inputBuffer.append(line);
            inputBuffer.append('\n');
        }

        reader.close();

        String inputStr = inputBuffer.toString();
        inputStr = inputStr.substring(0, inputStr.length()  - lastLine.length() - 1); // Supprime les deux dernières lignes

        BufferedWriter writer = new BufferedWriter(new FileWriter(fileName));
        writer.write(inputStr);
        writer.close();
    }
}