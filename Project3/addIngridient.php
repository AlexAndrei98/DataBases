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

//----------------------sql connected-------------------------------

    $searchQuery= "select Ingredient from andreia.Inventory where Ingredient = \"$ingridient\" ;";
    $search= mysqli_query($conn, $searchQuery);
        if (!$search)
            die("Error adding to the Inventory table : " . mysqli_error($conn));


    $row = mysqli_fetch_assoc($search);      
    echo "<br>";
    echo $row["Ingredient"];
    echo "<br>";
    //check if ingirident is into the database

    if($row["Ingredient"]==$ingridient){
        $updateQuery="update andreia.Inventory set Quantity=Quantity +\"$quantity\" where Ingredient=\"$ingridient\";";
        echo $updateQuery;
        $status = mysqli_query($conn, $updateQuery);
        if (!$status)
            die("Query Error : " . mysqli_error($conn));

    }
    else{
        $queryString = "insert into andreia.Inventory values (\"$ingridient\",\"$quantity\")";
        $status = mysqli_query($conn, $queryString);
        if (!$status)
            die("Error adding to the Inventory table : " . mysqli_error($conn));
    }

    
?>
<p>Back to <a href="mainMenu.php">the main menu</a>.</p>
