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
?>


