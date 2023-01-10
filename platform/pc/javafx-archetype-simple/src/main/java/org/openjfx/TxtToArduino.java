package org.openjfx;

import java.io.*;

public class TxtToArduino {

    public static void main() throws IOException {
        // Ouvre un fichier input.txt existant
        BufferedReader inputReader = new BufferedReader(new FileReader("output.txt"));
        // Crée un fichier output appelé codeArduino.txt
        BufferedWriter outputWriter = new BufferedWriter(new FileWriter("codeArduino.txt"));

        String Line;
        // Lit chaque ligne du fichier input
        while ((Line = inputReader.readLine()) != null) {

            // Trouve l'index du premier caractère "<" dans la ligne
            int indexOfLessThan = Line.indexOf('<');
            // Récupère les 4 premiers caractères qui suivent le caractère "<"
            String firstFourChars = Line.substring(indexOfLessThan + 1, indexOfLessThan + 5);

            // Si les 4 premiers caractères sont "path", appelle la méthode appropriée
            switch (firstFourChars) {
                case "path" -> {
                    outputWriter.write(extractOnePath(Line));

                }
                // Ajoute une ligne vide après chaque méthode appelée
                case "line" -> {
                    outputWriter.write(extractLinePath(Line));

                }
                case "rect" -> {
                    outputWriter.write(extractRectPath(Line));

                }
                case "elli" -> {
                    outputWriter.write(extractElliPath(Line));

                }
                case "poly" -> {
                    outputWriter.write(extractPolyPath(Line));

                }
            }
        }
        // Ferme les fichiers
        inputReader.close();
        outputWriter.close();
    }

    private static String extractPolyPath(String textPath) throws IOException {
        String x1 = "", y1 = "", x2 = "", y2 = "", pointxy0 = "", pointxy0Next = "";
        int posSpace, pos1, theFirst;
        double x0Value = 0, x1Value = 0, x2Value = 0, y0Value = 0, y1Value = 0, y2Value = 0;
        String text1 = null;
        String result = "";
        theFirst = 0;
        pos1 = textPath.indexOf("ts=");
        text1 = textPath.substring(pos1 + 4);

        while (text1.length() > 4) {
            pos1 = text1.indexOf(",");
            x1 = text1.substring(0, pos1);
            text1 = text1.substring(pos1);

            pos1 = text1.indexOf(" ");
            y1 = text1.substring(1, pos1);
            text1 = text1.substring(pos1);

            if (theFirst > 0) {
                x2Value = Double.parseDouble(x1);
                y2Value = Double.parseDouble(y1);

                result += "draw_line(0,0," + (x2Value - x1Value) + "," + (y2Value - y1Value) + ");\n";
            } else {
                result += "move_pen_abs(" + x1 + "," + y1 + ");\n";

                x0Value = Double.parseDouble(x1);
                y0Value = Double.parseDouble(y1);
            }

            x1Value = Double.parseDouble(x1);
            y1Value = Double.parseDouble(y1);
            theFirst = theFirst + 1;
            if (text1.length() <= 4) {
                result += "draw_line(0,0," + (-x1Value + x0Value) + "," + (-y1Value + y0Value) + ");\n";
            }
        }
        return result;
    }

    private static String extractElliPath(String textPath) throws IOException {
        // Declare variables to store the extracted ellipse properties
        String cxResult = "", cyResult = "", rxResult = "", ryResult = "", text1 = "", textResult = "";
        int pos1;
        // Assign textPath to text1
        text1 = textPath;

        // Extract cx property
        pos1 = text1.indexOf("cx=");
        text1 = text1.substring(pos1 + 3);
        pos1 = text1.indexOf(" cy");
        cxResult = text1.substring(0, pos1 - 3);

        // Extract cy property
        pos1 = text1.indexOf("cy=");
        text1 = text1.substring(pos1 + 3);
        pos1 = text1.indexOf(" rx");
        cyResult = text1.substring(0, pos1 - 3);

        // Extract rx property
        pos1 = text1.indexOf("rx=");
        text1 = text1.substring(pos1 + 3);
        pos1 = text1.indexOf(" ry");
        rxResult = text1.substring(0, pos1 - 3);

        // Extract ry property
        pos1 = text1.indexOf("ry=");
        text1 = text1.substring(pos1 + 3);
        pos1 = text1.indexOf("/");
        ryResult = text1.substring(0, pos1 - 2);

        // Construct the result string with the extracted ellipse properties
        textResult = "draw_ellipse(" + cxResult + "," + cyResult + "," + rxResult + "," + ryResult + ");\n";

        // Return the result string
        return textResult;
    }

    public static String extractRectPath(String text_path) throws IOException {
        // Initialize variables to hold various parts of the final result string
        String textResult = "";
        String text1 = "", pointx1 = "", pointy1 = "", pointx2 = "", pointy2 = "", widthResult = "", heightResult = "";
        // Initialize variables for storing intermediate calculation values
        int pos1;
        double x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0, x4 = 0, y4 = 0, homeX = 0, homeY = 0;

        // Store the input string in a variable for easier manipulation
        text1 = text_path;

        // Find the position of the first "x" in the string
        pos1 = text1.indexOf("x");
        // Get the part of the string after the "x"
        text1 = text1.substring(pos1 + 3);
        // Find the position of the first "y" after the "x"
        pos1 = text1.indexOf("y");
        // Get the substring between the "x" and the "y"
        pointx1 = text1.substring(0, pos1 - 2);

        // Find the position of the first "y" in the string
        pos1 = text1.indexOf("y");
        // Get the part of the string after the "y"
        text1 = text1.substring(pos1 + 3);
        // Find the position of the first "w" after the "y"
        pos1 = text1.indexOf("w");
        // Get the substring between the "y" and the "w"
        pointy1 = text1.substring(0, pos1 - 2);

        // Find the position of the first "w" in the string
        pos1 = text1.indexOf("w");
        // Get the part of the string after the "w"
        text1 = text1.substring(pos1 + 7);
        // Find the position of the first "h" after the "w"
        pos1 = text1.indexOf("h");
        // Get the substring between the "w" and the "h"
        widthResult = text1.substring(0, pos1 - 2);

        // Find the position of the first "h" in the string
        pos1 = text1.indexOf("h");
        // Get the part of the string after the "h"
        text1 = text1.substring(pos1 + 8);
        // Find the position of the first "/" after the "h"
        pos1 = text1.indexOf("/");
        // Get the substring between the "h" and the "/"
        heightResult = text1.substring(0, pos1 - 1);

        // Convert the extracted pointx1 and pointy1 values to double and store them in homeX and homeY
        homeX = Double.parseDouble(pointx1);
        homeY = Double.parseDouble(pointy1);

        // Calculate the coordinates of the other three points of the rectangle based on the width and height values
        x2 = Double.parseDouble(widthResult);
        y2 = 0;

        x3 = 0;
        y3 = Double.parseDouble(heightResult);

        x4 = -Double.parseDouble(widthResult);
        y4 = 0;

        x1 = 0;
        y1 = -Double.parseDouble(heightResult);

// Construct the strings representing the commands to move the pen and draw the lines
        String textResult1 = "move_pen_abs(" + homeX + "," + homeY + ");\n";
        String textResult2 = "draw_line(0,0," + x2 + "," + y2 + ");\n";
        String textResult3 = "draw_line(0,0," + x3 + "," + y3 + ");\n";
        String textResult4 = "draw_line(0,0," + x4 + "," + y4 + ");\n";
        String textResult5 = "draw_line(0,0," + x1 + "," + y1 + ");\n";

// Return the concatenation of all five strings as the final result
        return textResult1 + textResult2 + textResult3 + textResult4 + textResult5;
    }

    private static String extractLinePath(String text_path) throws IOException {
        // Declare variables to store the extracted line properties and intermediate results
        String result1 = "";
        String result2 = "";
        String result3 = "";
        String text1 = "";
        String pointx1 = "";
        String pointy1 = "";
        String pointx2 = "";
        String pointy2 = "";
        double x1 = 0;
        double x2 = 0;
        double y1 = 0;
        double y2 = 0;
        int pos1 = 0;

        // Assign text_path to text1
        text1 = text_path;

        // Extract x1 property
        pos1 = text1.indexOf("x1");
        text1 = text1.substring(pos1 + 3); // take string after x1
        pos1 = text1.indexOf("y1");
        pointx1 = text1.substring(0, pos1 - 3); // take string before y1, it is x1
        x1 = Double.parseDouble(pointx1);

        // Extract y1 property
        pos1 = text1.indexOf("y1");
        text1 = text1.substring(pos1 + 3); // take string after y1
        pos1 = text1.indexOf("x2");
        pointy1 = text1.substring(0, pos1 - 3); // take string before x2, it is y1
        y1 = Double.parseDouble(pointy1);

        // Extract x2 property
        pos1 = text1.indexOf("x2");
        text1 = text1.substring(pos1 + 3); // take string after x2
        pos1 = text1.indexOf("y2");
        pointx2 = text1.substring(0, pos1 - 3); // take string before y2, it is x2
        x2 = Double.parseDouble(pointx2);

        // Extract y2 property
        pos1 = text1.indexOf("y2");
        text1 = text1.substring(pos1 + 3); // take string after y2
        pos1 = text1.indexOf("/");
        pointy2 = text1.substring(0, pos1 - 3); // take string before /, it is y2
        y2 = Double.parseDouble(pointy2);

        // Construct the first result string
        result1 = "move_pen_abs(" + x1 + "," + y1 + ");";

        // Calculate x2 and y2 relative to x1 and y1
        x2 = x2 - x1;
        y2 = y2 - y1;

        // Construct the second result string
        result2 = "draw_line(" + pointx1 + "," + pointy1 + "," + pointx2 + "," + pointy2 + ");";

        // Construct the third result string
        result3 = "draw_line(0,0," + x2 + "," + y2 + ");";

        // Return the concatenation of all three result strings, separated by newlines
        return result1 + result2 + result3;
    }

    static StringBuilder textRemain = new StringBuilder("");

    public static String extractOnePath(String line) throws IOException {
        // Declare variables to store intermediate results and extracted path properties
        String textPickup = line;

        String textResult = "";
        StringBuilder result1 = new StringBuilder("");
        String point0Xy = "";
        String textFirst = "";
        int textRemainLength = 0;
        String result = "";
        String result2 = "";

        // Extract the first absolute move command and store the remaining string in textRemain
        result += extractMove(textPickup, textRemain);
        // Calculate the length of textRemain
        textRemainLength = textRemain.length();
        // Extract the starting point of the path from the move command
        point0Xy = result.substring(13, result.length() - 3);

        // Continue processing while textRemain is not empty
        while (textRemainLength > 3) {
            // Extract the next path command and store the remaining string in textRemain
                result1 = new StringBuilder(extractValue(textRemain, textRemain));
            // Extract the first letter of the command
            textFirst = result1.substring(0, 1);

            // Determine the type of command and extract the corresponding path properties
            switch (textFirst) {
                case "l":
                    result += extractLine(point0Xy, result1);
                    break;
                case "c":
                    result += extractCurve(point0Xy, result1);
                    break;
                case "m":
                    result += extractMoveRelative(point0Xy, result1);
                    break;
            }

            // Assign the remaining string to textPickup for further processing
            textPickup = String.valueOf(result1)+String.valueOf(textRemain);
            // Calculate the length of textRemain
            textRemainLength = textRemain.length();

        }
        // Return the result
        return result;
    }

    static int counter = 0;

    public static StringBuilder extractValue(StringBuilder textAll, StringBuilder textRemainValue) {
        int firstCaractere = textAll.indexOf("");
        int le= textAll.length();
        String textValue = textAll.substring(firstCaractere, le);


        int pos1 = textValue.indexOf("l");
        int pos2 = textValue.indexOf("c");
        int pos3 = textValue.indexOf("m");
        int pos4 = textValue.indexOf("/") - 1;
        if (pos1 == -1) {
            pos1 = 22000;
        }
        if (pos2 == -1) {
            pos2 = 22000;
        }
        if (pos3 == -1) {
            pos3 = 22000;
        }
        if (pos4 == -1) {
            pos4 = 22000;
        }
        int posNext = Math.min(pos1, Math.min(pos2, Math.min(pos3, pos4)));
        int posLatest = Math.min(pos2, Math.min(pos3, pos4));
        StringBuilder text2 = new StringBuilder(textValue.substring(posNext, posLatest)); //take the string to the next l or c
        textRemainValue = new StringBuilder(textValue.substring(posLatest, textValue.length())); // take the string from the next l or c to the end
        textRemain = textRemainValue;
        return text2;
    }
    public static String textAfterM = "";
    public static String extractMove(String allMoves, StringBuilder remaining) {
        if (allMoves == null || allMoves.isEmpty()) {
            return "";
        }

        // Find the start of the move command
        int startIndex = allMoves.indexOf("d=");
        if (startIndex < 0) {
            // No move command found
            return "";
        }
        startIndex += 4;  // Skip "d=M"

        // Find the end of the move command
        int endIndex = allMoves.length();  // End of the string
        char[] terminators = {'c', 'l', 'm'};  // Possible terminators of the move command
        for (char c : terminators) {
            int index = allMoves.indexOf(c, startIndex);
            if (index >= 0) {
                endIndex = Math.min(endIndex, index);  // Take the earliest terminator
            }
        }

        // Extract the move command and update the remaining string
        String move = allMoves.substring(startIndex, endIndex);
        move = move.replace(" ", ",");  // Replace spaces with commas
        remaining.replace(0, remaining.length(), allMoves.substring(endIndex));  // Update the remaining string
        textRemain = remaining;
        textAfterM = String.valueOf(remaining);
        return "move_pen_abs(" + move + ");\n";  // Result for Arduino: Move to x,y
    }

    public static StringBuilder extractCurve(String point0xy, StringBuilder textAll) throws IOException {
// Remaining text to be processed
        String textRemain;
// Temporary text to hold current value
        String textTemp;
// Resulting string for current iteration
        String textResult;
// Position of nearest space character
        int posSpace;
// Position of "z" character
        int posZ;
// Position of next space or "z" character
        int posNext;
// Counter to keep track of Bezier curve points
        int posMemo;
// String to hold result of method
        StringBuilder curveString = new StringBuilder("");
// Remove first "c" character from textAll
        textTemp = textAll.substring(1);
// Set initial value for posSpace to begin loop in next step
        posSpace = 1;
// Set initial value for posMemo to keep track of Bezier curve points
        posMemo = 0;

// Loop until no more space characters are found
        while (posSpace > 0) {
            // Find nearest space character
            posSpace = textTemp.indexOf(" ");
            // Set value of posNext to posSpace
            posNext = posSpace;

            // If no space character is found, check for "z" character
            if (posSpace == 0) {
                // Find "z" character if no space character is found (end of text)
                posZ = textTemp.indexOf("z");
                // Set value of posNext to posZ
                posNext = posZ;
            }

            // Take remaining text after current value
            textRemain = textTemp.substring(posNext);
            // Check if "z" character is at the end or if there is a space character following it
            if (posNext != 0) {
                // Case where "z" character is at the end or there is a space character following it
                textResult = textTemp.substring(0, posNext - 1);
            } else {
                // Case of end of textAll, no "z" character, no space character
                textResult = textTemp.substring(0, textTemp.length());
            }

            // Use posMemo to keep track of Bezier curve points
            switch (posMemo) {
                case 0 ->
                        // First point of Bezier curve
                        curveString.append("draw_curve(").append(point0xy).append(",").append(textResult);
                case 1 ->
                        // Second point of Bezier curve
                        curveString.append(textResult);
                case 2 ->
                        // Third point of Bezier curve
                        curveString.append(textResult).append(");\n");
            }

            // Increment posMemo
            posMemo++;
            // If posMemo is equal to 3, reset to 0 and set point0xy to current value
            if (posMemo == 3) {
                posMemo = 0;
                point0xy = textResult;
            }

            // Save remaining text for next iteration
            textTemp = textRemain;
        }

        return curveString;
    }


    public static String extractLine(String point0xy, StringBuilder textAll) {
        String textTempLine = "";
        String textResultLine = "";
        String textResultXLine = "";
        String textResultYLine = "";
        String point0xyNextLine = "";
        int posSpaceLine = 0;
        int posZLine = 0;
        int posNextLine = 0;
        int posMemoLine = 0;
        int pointXLine = 0;
        int pointYLine = 0;
        String activeCellValueLine = "";

        textTempLine = textAll.substring(1); // delete first letter "l"
        point0xy = point0xy.replace(" ", ",");
        point0xyNextLine = point0xy;
        posSpaceLine = 1; // to ignite loop function in next step
        posMemoLine = 0; // to memory position of point of line
        while (posSpaceLine > 0) {
            posSpaceLine = textTempLine.indexOf(" ");
            posNextLine = posSpaceLine;
            if (posSpaceLine == 0) {
                posZLine = textTempLine.indexOf("z");
                posNextLine = posZLine;
            }
            if (posNextLine <= 0) {
                textAll.replace(0, textAll.length(), textTempLine.substring(posNextLine));
                textResultLine = textTempLine.substring(0, posNextLine);
            } else {
                textResultLine = textTempLine.substring(0, textTempLine.length());

            }
            switch (posMemoLine) {
                case 0:
                    textResultXLine = textResultLine;
                    break;
                case 1:
                    textResultYLine = textResultLine;
                    break;
            }
            posMemoLine += 1;
            if (posMemoLine == 2) {
                posMemoLine = 0;
                point0xyNextLine = textResultXLine + "," + textResultYLine;
                point0xy = point0xyNextLine;
            }
            if (posZLine > 0) {
                activeCellValueLine += "draw_line(" + point0xyNextLine + ",0,0);\n";
            } else {
                activeCellValueLine += "draw_line(" + point0xyNextLine + "," + textResultLine + ");\n";
            }
            textTempLine = textAll.toString();
            if (textResultLine == textResultXLine || textResultLine == textResultXLine ){
                posSpaceLine = -1;
            }else {
                posSpaceLine = 1;
            }
            // posSpaceLine = textTempLine.indexOf(" ", posNextLine); // Mettre à jour la valeur de posSpaceLine
        }
        return activeCellValueLine;
    }

            public static String extractMoveRelative(String point0xy, StringBuilder textAll) throws IOException {
        // Déclare les variables pour stocker les résultats intermédiaires et les propriétés du chemin extrait
        String textTemp = "";
        String textResult = "";
        String textResultX = "";
        String textResultY = "";
        String point0xyNext = "";
        int posSpace = 1;
        int posZ = 0;
        int posNext = 0;
        int posMemo = 0;

        // Supprime la première lettre "m"
        textTemp = textAll.substring(1);
        // Remplace les espaces par des virgules
        point0xy = point0xy.replace(" ", ",");
        point0xyNext = point0xy;

        // Continue de traiter tant que posSpace est positif ou nul
        while (posSpace >= 0) {
            // Trouve l'espace le plus proche
            posSpace = textTemp.indexOf(" ");
            // Trouve la lettre z
            posZ = textTemp.indexOf("z");
            // Prend le plus petit des deux (si l'un des deux est -1, cela ne change rien)
            posNext = Math.min(posSpace, posZ);

            if (posNext != 0) {
                // Cas où la lettre z se trouve à la fin ou s'il y a un espace suivant
                textResult = textTemp.substring(0, posNext);
            } else {
                // Cas où la fin de textAll n'a pas de lettre z, pas d'espace
                textResult = textTemp;
            }

            // Détermine la prochaine action en fonction de la valeur de posMemo
            switch (posMemo) {
                case 0 -> textResultX = textResult;
                case 1 -> textResultY = textResult;
            }

            // Incrémente posMemo
            posMemo++;

            // Si posMemo atteint la valeur 2, remet la valeur à 0 et met à jour point0xy et point0xyNext
            if (posMemo == 2) {
                posMemo = 0;
                point0xyNext = textResultX + "," + textResultY;
                point0xy = point0xyNext;
            }

            // Met à jour textRemain et textTemp
            textRemain = new StringBuilder(textTemp.substring(posNext + 1));
            textTemp = String.valueOf(textRemain);
        }

        // Retourne le résultat sous la forme "move_pen_rel(x, y);"
        return "move_pen_rel(" + point0xy + ");\n";
    }
}

