
package org.openjfx;


import java.io.PrintWriter;
import java.util.*;
import java.io.*;

public class txtToArduino {
    private static String textRemain;
    public static void main()
    {
        try (Scanner input = new Scanner (new File ("output.txt")))
        {
            try (PrintWriter fileForESP = new PrintWriter (new File ("codeArduino.txt")))
            {
                while (input.hasNextLine ())
                {
                    String text_path = input.nextLine ();
                    int text_pos = text_path.indexOf ("<");
                    text_path = text_path.substring (text_pos);	// Prend la chaC.ne aprC(s le caractC(re "<"
                    String draw_type = text_path.substring (0, 4);	// Prend les 4 lettres aprC(s "<" pour détecter le type de dessin : path, line, rect, ...

                    if (draw_type.equals ("pat"))
                    {
                        fileForESP.write (extractOnePath (text_path));
                    }
                    else if (draw_type.equals ("lin"))
                    {
                        fileForESP.write (extractLinePath (text_path));
                    }
                    else if (draw_type.equals ("rec"))
                    {
                        fileForESP.write (extractRectPath (text_path));
                    }
                    else if (draw_type.equals ("ell"))
                    {
                        fileForESP.write (extractElliPath (text_path));
                    }
                    else if (draw_type.equals ("pol"))
                    {
                        fileForESP.write (extractPolyPath (text_path));
                    }

                }
            }
        }
        catch (FileNotFoundException e)
        {
            e.printStackTrace ();
        }
    }

    /**
     * @param textAll
     * @param textRemain
     * @return
     */
    public static String extractValue (String textAll, String textRemain)
    {
        int pos1, pos2, pos3, pos4, pos_latest;
        String text1, text2;

        text1 = textAll.substring (1);	// prendre la chaC.ne aprC(s la premiC(re lettre

        pos1 = text1.indexOf ("l");
        pos2 = text1.indexOf ("c");
        pos3 = text1.indexOf ("m");
        pos4 = text1.indexOf ("/") - 1;
        if (pos1 == 0)
        {
            pos1 = 22000;		// si pas de lettre "l" -> aucune importance
        }
        if (pos2 == 0)
        {
            pos2 = 22000;		// si pas de lettre "c" -> aucune importance
        }
        if (pos3 == 0)
        {
            pos3 = 22000;		// si pas de lettre "m" -> aucune importance
        }
        if (pos4 == 0)
        {
            pos4 = 22000;		// si pas de lettre "/" -> aucune importance
        }

        pos_latest = Math.min (pos1, Math.min (pos2, Math.min (pos3, pos4)));	// position de la prochaine lettre

        text2 = textAll.substring (0, pos_latest);	// chaC.ne allant du dC)but de text_all jusqu'C  la prochaine lettre c ou l
        textRemain = text1.substring (pos_latest - 1);	// chaC.ne allant de la lettre c ou l jusqu'C  la fin

        return text2;
    }

    /**
     * @param textAll
     * @param textRemain
     * @return
     */
    public static String extractMove (String textAll, String textRemain)
    {
        int pos1, pos2, pos3, pos4;
        String text1, text2;
        pos1 = textAll.indexOf ("d=");
        text1 = textAll.substring (pos1 + 3);	//take string after d="M

        pos1 = text1.indexOf ("l");
        pos2 = text1.indexOf ("c");
        pos3 = text1.indexOf ("m");
        if (pos1 == 0)
        {
            pos1 = 22000;		//if no letter "l" -> no care
        }
        if (pos2 == 0)
        {
            pos2 = 22000;		//if no letter "c" -> no care
        }
        if (pos3 == 0)
        {
            pos3 = 22000;		//if no letter "m" -> no care
        }

        pos4 = Math.min (pos1, Math.min (pos2, pos3));	//position of next letter

        text2 = text1.substring (0, pos4 - 1);	//string from M to next letter c or l
        textRemain = text1.substring (pos4 - 1);	//string from letter c or l to end

        text2 = text2.replace (" ", ",");

        return "move_pen_abs(" + text2 + ");";	//result for arduino: Move to x,y
    }

    /**
     * @param point0xy
     * @param textAll
     */
    public static void extractCurve (String point0xy, String textAll)
    {
        String textRemain, textTemp, textResult;
        int posSpace, posZ, posNext, posMemo;

        textTemp = textAll.substring (1);	// Supprime la premiC(re lettre "c"
        posSpace = 1;		// Pour dC)marrer la boucle dans l'C)tape suivante
        posMemo = 0;		// Pour mC)moriser la position du point de la courbe Bezier

        while (posSpace > 0)
        {
            posSpace = textTemp.indexOf (" ");	// Trouve l'espace le plus proche
            posNext = posSpace;

            if (posSpace == 0)
            {
                posZ = textTemp.indexOf ("z");	// Trouve la lettre z si pas d'espace (fin de texte)
                posNext = posZ;
            }

            textRemain = textTemp.substring (posNext);
            if (posNext != 0)
            {
                // Cas oC9 la lettre z est C  la fin ou a un espace suivant
                textResult = textTemp.substring (0, posNext - 1);
            }
            else
            {
                // Cas de fin de texteAll, pas de lettre z, pas d'espace
                textResult = textTemp.substring (0, textTemp.length ());
            }

            switch (posMemo)
            {
                case 0:
                    System.out.println ("draw_curve(" + point0xy + "," + textResult);
                    break;
                case 1:
                    System.out.println (textResult);
                    break;
                case 2:
                    System.out.println (textResult + ");");
                    break;
            }

            posMemo++;
            if (posMemo == 3)
            {
                posMemo = 0;
                point0xy = textResult;
            }

            textTemp = textRemain;	// Enregistre le texte restant pour la prochaine boucle
        }

    }

    /**
     * @param point0xy
     * @param textAll
     */
    public static void extractLine (String point0xy, String textAll)
    {
        String text_remain = "";
        String text_temp = "";
        String text_result = "";
        String text_result_x = "";
        String text_result_y = "";
        String point0xy_next = "";
        int pos_space = 0;
        int pos_z = 0;
        int pos_next = 0;
        int pos_memo = 0;

        text_temp = textAll.substring (1);	//delete first letter "c"

        point0xy = point0xy.replace (" ", ",");
        point0xy_next = point0xy;

        pos_space = 1;		//to ignite loop function in next step
        pos_memo = 0;		// to memory position of point of line

        while (pos_space > 0)
        {
            pos_space = text_temp.indexOf (" ");	//find nearest space
            pos_next = pos_space;
            if (pos_space == 0)
            {
                pos_z = text_temp.indexOf ("z");	//find z letter if no space (means at the end text)
                pos_next = pos_z;
            }

            text_remain = text_temp.substring (pos_next);
            if (pos_next != 0)
            {
                //case letter z at the end/ or next space
                text_result = text_temp.substring (0, pos_next - 1);
            }
            else
            {
                //case end of text_all no letter z, no space
                text_result = text_temp.substring (0, text_temp.length ());
            }

            switch (pos_memo)
            {
                case 0:
                    System.out.println ("draw_line(" + point0xy_next + "," + text_result);	//print out value
                    text_result_x = text_result;
                    break;
                case 1:
                    System.out.println (text_result + ");");
                    text_result_y = text_result;
                    break;
            }

            pos_memo = pos_memo + 1;
            if (pos_memo == 2)
            {
                pos_memo = 0;
                point0xy_next = text_result_x + "," + text_result_y;
                point0xy = point0xy_next;
            }

            if (pos_z > 0)
            {
                //print out z point
                System.out.println ("draw_line(" + point0xy_next + ",0,0);");
            }

            text_temp = text_remain;	//save remain text for next loop
        }
    }

    /**
     * @param point0xy
     * @param textAll
     */
    public static void extractMoveRelative (String point0xy, String textAll)
    {
        String text_remain = "";
        String text_temp = "";
        String text_result = "";
        String text_result_x = "";
        String text_result_y = "";
        String point0xy_next = "";
        int pos_space = 1;
        int pos_z = 0;
        int pos_next = 0;
        int pos_memo = 0;


        text_temp = textAll.substring (1);	//delete first letter "m"
        point0xy = point0xy.replace (" ", ",");
        point0xy_next = point0xy;

        while (pos_space > 0)
        {
            pos_space = text_temp.indexOf (" ");	//find nearest space
            pos_next = pos_space;

            if (pos_space == 0)
            {
                pos_z = text_temp.indexOf ("z");	//find z letter if no space (means at the end text)
                pos_next = pos_z;
            }

            if (pos_next != 0)
            {
                //case letter z at the end/ or next space
                text_result = text_temp.substring (0, pos_next - 1);
            }
            else
            {
                //case end of text_all no letter z, no space
                text_result = text_temp;
            }

            switch (pos_memo)
            {
                case 0:
                    System.out.println ("move_pen_rel(" + text_result);	//print out value
                    text_result_x = text_result;
                    break;
                case 1:
                    System.out.println (text_result + ");");
                    text_result_y = text_result;
                    break;
            }

            pos_memo++;
            if (pos_memo == 2)
            {
                pos_memo = 0;
                point0xy_next = text_result_x + "," + text_result_y;
                point0xy = point0xy_next;
            }

            if (pos_z > 0)
            {
                //print out z point
                System.out.println ("move_pen_rel(" + point0xy + ");");	//print out value
            }

            text_temp = text_remain;	//save remain text for next loop
        }
    }

    /**
     * @param text_path
     * @return
     */
    private static String extractPolyPath (String text_path)
    {
        String x1 = "", y1 = "", x2 = "", y2 = "", pointxy0 = "", pointxy0Next =
                "";
        int posSpace, pos1, theFirst;
        double x0Value = 0, x1Value = 0, x2Value = 0, y0Value = 0, y1Value =
                0, y2Value = 0;
        String text1 = "";
        String result1 = "", result2 = "", result3 = "";
        theFirst = 0;
        pos1 = text_path.indexOf ("ts=");
        text1 = text_path.substring (pos1 + 3);

        while (text1.length () > 4)
        {
            pos1 = text1.indexOf (",");
            x1 = text1.substring (0, pos1 - 1);
            text1 = text1.substring (pos1 + 0);

            pos1 = text1.indexOf (" ");
            y1 = text1.substring (0, pos1 - 1);
            text1 = text1.substring (pos1 + 0);

            if (theFirst > 0)
            {
                x2Value = Double.parseDouble (x1);
                y2Value = Double.parseDouble (y1);

                result1 =
                        "draw_line(0,0," + (x2Value - x1Value) + "," + (y2Value -
                                y1Value) +
                                ");\n";
            }
            else
            {
                result2 = "move_pen_abs(" + x1 + "," + y1 + ");\n";

                x0Value = Double.parseDouble (x1);
                y0Value = Double.parseDouble (y1);
            }

            x1Value = Double.parseDouble (x1);
            y1Value = Double.parseDouble (y1);
            theFirst = theFirst + 1;

            if (text1.length () <= 4)
            {
                result3 = "draw_line(0,0," + (-x1Value + x0Value) + "," + (-y1Value + y0Value) + ");\n";
            }
        }

        return result1 + result2 + result3;

    }

    /**
     * @param text_path
     * @return
     */
    private static String extractElliPath (String text_path)
    {
        String cxResult = "", cyResult = "", rxResult = "", ryResult = "", text1 =
                "", textResult = "";
        int pos1;
        text1 = text_path;

        pos1 = text1.indexOf ("x");
        text1 = text1.substring (pos1 + 2);
        pos1 = text1.indexOf ("c");
        cxResult = text1.substring (0, pos1 - 3);

        pos1 = text1.indexOf ("y");
        text1 = text1.substring (pos1 + 2);
        pos1 = text1.indexOf ("r");
        cyResult = text1.substring (0, pos1 - 3);

        pos1 = text1.indexOf ("x");
        text1 = text1.substring (pos1 + 2);
        pos1 = text1.indexOf ("r");
        rxResult = text1.substring (0, pos1 - 3);

        pos1 = text1.indexOf ("y");
        text1 = text1.substring (pos1 + 2);
        pos1 = text1.indexOf ("/");
        ryResult = text1.substring (0, pos1 - 2);

        textResult =
                "draw_ellipse(" + cxResult + "," + cyResult + "," + rxResult + "," +
                        ryResult + ");";

        return textResult;
    }

    /**
     * @param textPathValue
     * @return
     */
    public static String extractOnePath (String textPathValue)
    {
        String textPickup = textPathValue;
        String textMoveAbs = "";
        String textRemain = "";
        String textResult = "";
        String point0Xy = "";
        String textFirst = "";
        int textRemainLength = 0;
        String result;

        textResult = extractMove (textPickup, textRemain);
        textPickup = textRemain;
        textRemainLength = textRemain.length ();
        point0Xy = textResult.substring (13, textResult.length () - 2);

        while (textRemainLength > 3)
        {
            textResult = extractValue (textPickup, textRemain);
            textFirst = textResult.substring (0, 1);

            switch (textFirst)
            {
                case "l":
                    extractLine (point0Xy, textResult);
                    break;
                case "c":
                    extractCurve (point0Xy, textResult);
                    break;
                case "m":
                    extractMoveRelative (point0Xy, textResult);
                    break;
            }

            textPickup = textRemain;
            textRemainLength = textRemain.length ();
        }
        return "a";
    }


    /**
     * @param text_path
     * @return
     */
    private static String extractLinePath (String text_path)
    {
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

        text1 = text_path;

        pos1 = text1.indexOf ("x1");
        text1 = text1.substring (pos1 + 3);	// take string after x1
        pos1 = text1.indexOf ("y1");
        pointx1 = text1.substring (0, pos1 - 3);	// take string before y1, it is x1
        x1 = Double.parseDouble (pointx1);

        pos1 = text1.indexOf ("y1");
        text1 = text1.substring (pos1 + 3);	// take string after y1
        pos1 = text1.indexOf ("x2");
        pointy1 = text1.substring (0, pos1 - 3);	// take string before x2, it is y1
        y1 = Double.parseDouble (pointy1);

        pos1 = text1.indexOf ("x2");
        text1 = text1.substring (pos1 + 3);	// take string after x2
        pos1 = text1.indexOf ("y2");
        pointx2 = text1.substring (0, pos1 - 3);	// take string before y2, it is x2
        x2 = Double.parseDouble (pointx2);

        pos1 = text1.indexOf ("y2");
        text1 = text1.substring (pos1 + 3);	// take string after y2
        pos1 = text1.indexOf ("/");
        pointy2 = text1.substring (0, pos1 - 3);	// take string before /, it is y2
        y2 = Double.parseDouble (pointy2);

        result1 = "move_pen_abs(" + x1 + "," + y1 + ");";
        x2 = x2 - x1;
        y2 = y2 - y1;

        result2 =
                "draw_line(" + pointx1 + "," + pointy1 + "," + pointx2 + "," + pointy2 +
                        ");";
        result3 = "draw_line(0,0," + x2 + "," + y2 + ");";

        return "\n" + result1 + "\n" + result2 + "\n" + result3;
    }

    /**
     * @param text_path
     * @return
     */
    public static String extractRectPath (String text_path)
    {
        String textResult = "";
        String text1 = "", pointx1 = "", pointy1 = "", pointx2 = "", pointy2 =
                "", widthResult = "", heightResult = "";
        int pos1;
        double x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0, x4 = 0, y4 =
                0, homeX = 0, homeY = 0;
        text1 = text_path;

        pos1 = text1.indexOf ("x");
        text1 = text1.substring (pos1 + 2);
        pos1 = text1.indexOf ("y");
        pointx1 = text1.substring (0, pos1 - 3);

        pos1 = text1.indexOf ("y");
        text1 = text1.substring (pos1 + 2);
        pos1 = text1.indexOf ("w");
        pointy1 = text1.substring (0, pos1 - 3);

        pos1 = text1.indexOf ("w");
        text1 = text1.substring (pos1 + 6);
        pos1 = text1.indexOf ("h");
        widthResult = text1.substring (0, pos1 - 3);

        pos1 = text1.indexOf ("h");
        text1 = text1.substring (pos1 + 7);
        pos1 = text1.indexOf ("/");
        heightResult = text1.substring (0, pos1 - 2);

        homeX = Double.parseDouble (pointx1);
        homeY = Double.parseDouble (pointy1);

        x2 = Double.parseDouble (widthResult);
        y2 = 0;

        x3 = 0;
        y3 = Double.parseDouble (heightResult);

        x4 = -Double.parseDouble (widthResult);
        y4 = 0;

        x1 = 0;
        y1 = -Double.parseDouble (heightResult);

        String textResult1 = "move_pen_abs(" + homeX + "," + homeY + ");\n";
        String textResult2 = "draw_line(0,0," + x2 + "," + y2 + ");\n";
        String textResult3 = "draw_line(0,0," + x3 + "," + y3 + ");\n";
        String textResult4 = "draw_line(0,0," + x4 + "," + y4 + ");\n";
        String textResult5 = "draw_line(0,0," + x1 + "," + y1 + ");\n";

        return textResult1 + textResult2 + textResult3 + textResult4 +
                textResult5;
    }
}
