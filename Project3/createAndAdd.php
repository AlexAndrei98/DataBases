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
        $searchQuery= "select rName, Ingredient from andreia.Recipes where rName = \"$recepie\" and Ingredient = \"$ingridient\" ;";
        $search= mysqli_query($conn, $searchQuery);
            if (!$search)
                die("Error adding to the Inventory table : " . mysqli_error($conn));

        $row = mysqli_fetch_assoc($search);      

        //check if ingirident is into the database
        if($row["Ingredient"]==$ingridient){
            $updateQuery="update andreia.Recipes set Quantity=\"$quantity\" where Ingredient=\"$ingridient\" and rName=\"Alex\";";
            echo $updateQuery;
            $status = mysqli_query($conn, $updateQuery);
            if (!$status)
                die("Query Error : " . mysqli_error($conn));
        }
        else {  
            $insertQuery = "insert into andreia.Recipes values (\"$recepie\",\"$ingridient\",\"$quantity\")";
            echo $insertQuery;
            $status = mysqli_query($conn, $insertQuery);
                if (!$status)
                    die("    1 Query Error : " . mysqli_error($conn));
        }
        $isIngThere= "select count(Ingredient) as total from andreia.Inventory where Inventory.Ingredient=\"$ingridient\";";
        $status = mysqli_query($conn, $isIngThere);
        if (!$status)
            die("Error in finding the inggridient in the invetory table : " . mysqli_error($conn));
        $count = mysqli_fetch_assoc($status); 
        //check if ingirdients is in the inventory
        if ($count["total"]==0){
            //add that ingiridient to the inventory with a value of 0
            $initializeInventory = "insert into andreia.Inventory values (\"$ingridient\",\"0\")";
            $status = mysqli_query($conn, $initializeInventory);
            if (!$status)
                die("Error adding to the Inventory table : " . mysqli_error($conn));
        }

?>


<p>Back to <a href="mainMenu.php">the main menu</a>.</p>
