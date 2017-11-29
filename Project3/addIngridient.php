<?php 
    $recepie = $_POST["rName"];
    $ingridient = $_POST["Ingridient"];
    $quantity = $_POST["Quantity"];
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
        $queryString = "insert into andreia.Inventory values (\"$ingridient\",\"$quantity\")";
        $status = mysqli_query($conn, $queryString);
        if (!$status)
            die("Error adding to the Inventory table : " . mysqli_error($conn));
?>

