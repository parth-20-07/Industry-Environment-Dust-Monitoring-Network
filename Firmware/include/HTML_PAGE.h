#ifndef HTML_PAGE_H
#define HTML_PAGE_H
// HTML web page to handle input fields
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <title>Dust Monitor Configuration Page</title>
    <meta name="viewport" http-equiv="Content-Type"
        content="width=device-width, initial-scale=1 text/html; charset=utf-8">
    <style>
        body {
            background-color: #0000008c;
            font-family: Georgia, "Times New Roman", Times, serif;
            color: rgb(255, 255, 255);
        }

        input[type="text"] {
            width: 100px;
            height: 20px;
            border: 1px solid rgba(0, 0, 0, 0.349);
            background: #ffffff;
        }

        input[type="number"] {
            width: 100px;
            height: 20px;
            border: 1px solid rgba(0, 0, 0, 0.349);
            background: #ffffff;
        }
    </style>
</head>

<body>
    <center>
        <form action="/get">
            <h2 align="center">Dust Monitor Configuration Form</h2>
            <br>
            <table>
                <h3>Enter Wi-Fi Credentials</h3>
                <tr>
                    <td align="right">SSID:</td>
                    <td><input type="text" name="ssid" placeholder="Enter SSID" /></td>
                </tr>

                <tr>
                    <td align="right">Password:</td>
                    <td>
                        <input type="text" name="password" placeholder="Enter Password" />
                    </td>
                </tr>
                <br />
            </table>
            <input type="submit" value="Submit" />
        </form>
    </center>
</body>

</html>
)rawliteral";

#endif