<?php
// start session
session_start();
$_SESSION["host"] = $_POST["host"];
$_SESSION["user"] = $_POST["user"];
$_SESSION["passw"] = $_POST["password"];
$dbName = $_SESSION["user"];
// build the connection ...
$conn = mysqli_connect($_SESSION["host"], $_SESSION["user"], $_SESSION["passw"], $dbName);
//checking if the conncection is right
if (!$conn)
	die("Could not connect:".mysqli_connect_error());
else
	echo "Succesfully connected to ". $_SESSION["host"];
$_SESSION["logged"] = true;
$queryString = "create table if not exists Recipes (rName char(200) not null, Ingredient char(100) not null, Quantity integer, primary key (rName, Ingredient))";
$status = mysqli_query($conn, $queryString);
if (!$status)
    die("Error creating table: Recepies" . mysqli_error($conn));
$queryString = "create table if not exists Inventory (Ingredient char(100) not null, Quantity integer )";
$status = mysqli_query($conn, $queryString);
if (!$status)
    die("Error creating table: " . mysqli_error($conn));

$queryString = "alter table Inventory add check (Inventory.Ingredient >=0);";
$status = mysqli_query($conn, $queryString);
if (!$status)
    die("Error creating table: " . mysqli_error($conn));

mysqli_close($conn);
?>
<p>Continue to <a href="mainMenu.php">the main menu</a>.</p>