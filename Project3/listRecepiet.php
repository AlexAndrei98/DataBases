<?php 
$recepie = $_POST["rName"];
session_start();
$conn = mysqli_connect($_SESSION["host"], $_SESSION["user"], $_SESSION["passw"], $dbName);
if (!$_SESSION["logged"])
    header("Location: index.html");
    if (!$conn)
        die("Could not connect:".mysqli_connect_error());
    else
        echo "Succesfully connected to ". $_SESSION["host"];
//----------------------sql connected-------------------------------

    echo "<table border=1>";
    echo "<h1>Ingridents for $recepie</h1>";
    echo "<tr> <th>Ingridients</th> <th>Quantity</th> </tr>";

    $queryString = "select Ingredient,Quantity from andreia.Recipes where rName = \"$recepie\"";
    $status = mysqli_query($conn, $queryString);
    if (!$status)
        die("Error in getting the ingridients and : " . mysqli_error($conn));
        
    while($row = mysqli_fetch_assoc($status)){
        echo"<td>".$row["Ingredient"]."</td>". 
            "<td>".$row["Quantity"]."</td> </tr>";
    }
    echo "</table>";
?>

