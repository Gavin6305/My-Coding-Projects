<html>
    <head>
        <title>Weather Forecast for Camden</title>
        <link rel="stylesheet" href="styles.css">
    </head>
    <body>
        <h1>Weather Forecast for Camden</h1>
        <table align="center" border="1">
            <tr>
                <td class="date-col col-title">Day of week</td>
                <td class="dn-col col-title">Time of Day</td>
                <td class="temp-col col-title">Temperature</td>
                <td class="sd-col col-title">Brief Description</td>
                <td class="ld-col col-title">Detailed Description</td>
            </tr>
            <?php
                $cnx = new mysqli('localhost', 'root', 'balls', 'forecastDB');

                if ($cnx->connect_error)
                    die('Connection failed: ' . $cnx->connect_error);

                $query = 'SELECT * FROM Camden';
                $cursor = $cnx->query($query);
                while ($row = $cursor->fetch_assoc()) {
                    $darkenRow = $row['dayOrNight'] == 1 ? "day-row" : "night-row";
                    echo '<tr class="' . $darkenRow . '">';
                    echo '<td class="date-col">' . $row['date'] . '</td>';
                    echo '<td class="dn-col">' . ($row['dayOrNight'] == 1 ? "Daytime" : "Nighttime") . '</td>';
                    echo '<td class="temp-col">' . $row['temperature'] . '</td>';
                    echo '<td class="sd-col"><p>' . $row['shortDesc'] . '</p></td>';
                    echo '<td class="ld-col"><p>' . $row['longDesc'] . '</p></td>';
                    echo '</tr>';
                }
                $cnx->close();
            ?>
        </table>
        <h2>Select another city:</h2>
        <p class="links">
            <a href="elizabeth.php">Elizabeth</a>&emsp;
            <a href="milford.php">Milford</a>&emsp;
            <a href="phillipsburg.php">Phillipsburg</a>&emsp;
            <a href="vineland.php">Vineland</a>
        </p>
    </body>
</html>