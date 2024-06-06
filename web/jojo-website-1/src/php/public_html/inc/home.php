<?php
    require(INC_FOLDER . "config.php");
?>

<!DOCTYPE html>
<html>
<head>
<link rel="stylesheet" type="text/css" href="https://fonts.googleapis.com/css?family=Inconsolata"/>
<link rel="stylesheet" href="./styles/global.css">
<link rel="stylesheet" href="./styles/chat.css">
<link rel="icon" href="./images/favicon.ico" type="image/x-icon">
<title>Jojo Website</title>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
<?php include(INC_FOLDER . "header.php"); ?>
<section class="background-section">
    <?php include(INC_FOLDER . "background.php"); ?>
</section>

<div style="background-color=black;bottom: 1vh;padding: 0 1%;color: white;background: #000000a3;position: absolute;width: 97%;text-align: center;margin: auto;"> 
    <p>Hey, I am currently building this website in order for us to chat here. For now, you can already create you accounts.</p>
</div>
<div id="users">
  <h1>Users List</h1>
  <table>
    <thead>
      <tr>
        <th>ID</th>
        <th>Username</th>
      </tr>
    </thead>
    <tbody>
    <?php
    $sql = "SELECT id, username FROM users";
    if($stmt = mysqli_prepare($conn, $sql)){
        $stmt->execute();
        $stmt->store_result();
        if ($stmt->num_rows > 0) {
            $stmt->bind_result($id, $username);
            while ($stmt->fetch()) {
                # to change into tables
                echo "<tr><td>$id </td><td> ".htmlentities($username)."</td></tr>";
            }
        }
        $stmt->close();
    }
 ?>
     </tbody>
  </table>
 </div>

</body>