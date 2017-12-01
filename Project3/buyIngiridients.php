<?php 
    $recepie = $_POST["rName"];
	// check if session is open
    session_start();
    $conn = mysqli_connect($_SESSION["host"], $_SESSION["user"], $_SESSION["passw"], $dbName);
	// if the user is not logged in, redirect to login page
	if (!$_SESSION["logged"])
        header("Location: index.html");
        // build the connection ...
       
        //checking if the conncection is right
        if (!$conn)
            die("Could not connect:".mysqli_connect_error());
        else
            echo "Succesfully connected to ". $_SESSION["host"];
//----------------------sql connected-------------------------------

$Transaction= "START TRANSACTION;";
$statusT = mysqli_query($conn, $Transaction);
if (!$statusT)
    die("QUERY ERROR WHEN STARTING A TRANSACTION: " . mysqli_error($conn));
echo "<br>";
echo $Transaction;
$buyIngridients = "update andreia.Inventory, andreia.Recipes set Inventory.Quantity=Inventory.Quantity-Recipes.Quantity where Recipes.rName= \"$recepie\"and Inventory.Ingredient = Recipes.Ingredient;";

$statusB = mysqli_query($conn, $buyIngridients);
if (!$statusB)
    die("QUERY ERROR WHEN UPDATING THE INVENTORY: " . mysqli_error($conn));

echo "<br>";
echo $buyIngridients;
$negativeInventory="select  count(Inventory.Quantity) as number1 from andreia.Inventory where Inventory.Quantity<0;";
$statusI = mysqli_query($conn, $negativeInventory);
if (!$statusI)
    die("QUERY ERROR WHEN CHECKING IF NEGATIVE QUANTITIES EXISTS IN THE INVENTORY" . mysqli_error($conn));

$count = mysqli_fetch_assoc($statusI); 
echo "<br>";
echo $count["number1"] ;
if($count["number1"]!=0){
    $Transaction="ROLLBACK;";
    $status = mysqli_query($conn, $Transaction);
    if (!$status)
        die("QUERY ERROR WHEN DOING A ROLLBACK : " . mysqli_error($conn));
    echo "<p>\"Unsuccesful purchase aka ROLLBACK\"</p>";
}
else{
    $Transaction="COMMIT;";
    $status = mysqli_query($conn, $Transaction);
    if (!$status)
        die("QUERY ERROR WHEN DOING A ROLLBACK : " . mysqli_error($conn));
    echo "<p>\"Succesful purchase AKA COMMMIT\"</p>";

        
}
?>
<p>Back to <a href="mainMenu.php">the main menu</a>.</p>
