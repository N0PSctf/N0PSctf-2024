<?php
    require(INC_FOLDER . "config.php");

    if (isset($_GET["delete_user"])) {
        $username = $_GET["delete_user"];
        $sql = "DELETE FROM users WHERE username = ?";
        if($stmt = mysqli_prepare($conn, $sql)){
            $stmt->bind_param('s', $username);
            $stmt->execute();
            $stmt->close();
            $u_str = htmlentities($username);
            echo "<script>alert('User $u_str was successfully deleted');</script>";
        }
    }
?>

<!DOCTYPE html>
<html>
  <head>
    <link rel="stylesheet" type="text/css" href="https://fonts.googleapis.com/css?family=Inconsolata"/>
    <link rel="stylesheet" href="./styles/global.css">
    <link rel="icon" href="./images/favicon.ico" type="image/x-icon">
    <title>Admin section</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=true;">
  </head>
  <body>
  <?php include(INC_FOLDER . "header.php"); ?>
  <div id="message-notif">
    Here is your flag: N0PS{1d0R_p4zZw0rD_r3Z3t}  
  </div>
  <section class="background-section" style="overflow: hidden;">
        <?php include(INC_FOLDER . "background.php"); ?>
    </section>
    <div class="foreground">
        <div class="admin-logs">
        <h3>Users</h3>
        <ul>
            <?php
            $result = $conn->query("SELECT id, username, is_admin FROM users");
            while ($row = $result->fetch_assoc()) {
                $username = htmlentities($row['username']);
                if (!$row['is_admin']) {
                    echo("<li><b>$username</b> <a href='/?page=admin&delete_user=$username'>Delete user</a></li>");
                } else {
                    echo("<li><b>$username</b> <b>ADMIN</b>");
                }
            }
            ?>
        </ul>
        </div>
    </div>
</body>