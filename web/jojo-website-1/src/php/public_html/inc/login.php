<?php
    require(INC_FOLDER . "config.php");

    $incorrect = false;

    $username = $password = "";

    if (isset($_POST['username']) && isset($_POST['password'])) {
        $username = $_POST['username'];
        $password = $_POST['password'];
        $sql = "SELECT id, password, is_admin FROM users WHERE username = ?";
        if($stmt = mysqli_prepare($conn, $sql)){
            $stmt->bind_param('s', $username);
            $stmt->execute();
            $stmt->store_result();
            if ($stmt->num_rows > 0) {
                $stmt->bind_result($id, $password_hash, $is_admin);
                $stmt->fetch();
                if (password_verify($password, $password_hash)) {
                    $_SESSION['username'] = $username;
                    $_SESSION['id'] = $id;
                    if ($is_admin) {
                        $_SESSION['admin'] = 1;
                    }
                    header("Location: /");
                } else {
                    $incorrect = true;
                }
            } else {
                $incorrect = true;
            }
            $stmt->close();
        }
    }
?>


<!DOCTYPE html>
<html>
  <head>
    <link rel="stylesheet" type="text/css" href="https://fonts.googleapis.com/css?family=Inconsolata"/>
    <link rel="stylesheet" href="./styles/global.css">
    <link rel="icon" href="./images/favicon.ico" type="image/x-icon">
    <title>Login Portal</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
  </head>
  <body>
  <?php include(INC_FOLDER . "header.php"); ?>
  <div id="message-notif" <?php if (!$incorrect) echo 'style="display:none;"'; ?>>
    <?php if ($incorrect) echo 'Incorrect username and/or password!'; ?>
  </div>
  <section class="background-section">
    <?php include(INC_FOLDER . "background.php"); ?>
</section>
<section>
    <div>
        <h2>Login</h2>
        <form method='POST' action='/?page=login'>
            <div class="input">
                <label for="string">Username :</label>
                <input type="text" id="username" name="username" required>
            </div>
            <div class="input">
                <label for="string">Password :</label>
                <input type="password" id="password" name="password" required>
            </div>
            <button name="submit" type="submit">Login</button>
        </form>
        <a href="/?page=register"><button class="header-button">Create an account</button></a>
    </div>
  </section>
</body>