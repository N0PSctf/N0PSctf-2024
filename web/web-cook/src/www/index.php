<?php

    $logged = false;

    if (isset($_POST['username'])) {
        $username = $_POST["username"];
        $isAdmin = 0;
        $b64cookie = base64_encode(json_encode(array("username" => $username, "isAdmin" => $isAdmin)));
        setcookie("session", $b64cookie);
        $logged = true;
    }

    if (isset($_COOKIE["session"])) {
        try {
            $session_data = json_decode(base64_decode($_COOKIE["session"]), true);
        } catch (Exception $e) {
            echo "Are you trying to mess with my application ?";
            die();
        }

        if ($session_data == null) {
            echo "Are you trying to mess with my application?";
            die();
        }
        
        if (!array_key_exists("username", $session_data)) {
            echo "Are you trying to mess with my application ?";
            die();
        }
        if (!array_key_exists("isAdmin", $session_data)) {
            echo "Are you trying to mess with my application ?";
            die();
        }
        if (count($session_data) != 2) {
            echo "Are you trying to mess with my application ?";
            die();
        }
        $username = $session_data["username"];
        if ($username == "") {
            echo "Are you trying to mess with my application ?";
            die();
        }
        $isAdmin = $session_data["isAdmin"];
        if ($isAdmin != 0 && $isAdmin != 1) {
            echo "Are you trying to mess with my application ?";
            die();
        }
        $logged = true;
    }

?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Recipe - PHP cooking</title>
  <link rel="stylesheet" href="style.css">
</head>
<body>
  <header>
    <h1>Cookie-based PHP authentication</h1>
    <?php
        if (!$logged) {
    ?>
    <div id="username-input">
        <form id="userForm" method="POST" action="">
            <label for="username">Enter your username:</label>
            <input type="text" id="username" name="username">
            <button type="submit" class="custom-button">Submit</button>
        </form>
    </div>
    <?php 
        } else {
    ?>
    <div id="username-input">
        <h4>Hello <?php echo htmlentities($username);?>!</h4>
    </div>
    <?php
        }
    ?>
  </header>
  
  <section class="recipe-section">
    <div class="recipe-card">
      <h2>Ingredients:</h2>
      <ul>
        <li>Fresh PHP</li>
        <li>A cookie</li>
        <li>Bad security skills flavour</li>
        <li>Base64 ustensils</li>
        <li>Minced JSON</li>
        <li>Gullibility sauce</li>
        <li>Internet hoven</li>
      </ul>
    </div>
    
    <div class="recipe-card">
      <h2>Instructions:</h2>
      <ol>
        <li>Use the Base64 to mix the cookie with the JSON. You should obtain a PHP array.</li>
        <li>Use the fresh PHP seasoned with your gullibility to check if the isAdmin value of the cookie is set to 1.</li>
        <li>Next, add your bad security skills flavour to give the flag.</li>
        <li>Put it in the Internet hoven. After some minutes, you should see the results.</li>
      </ol>
    </div>
  </section>

  <?php
  if ($logged) {
    if ($isAdmin) {
      echo "<div style='color:red'>You are an admin! Here is your flag: N0PS{y0u_Kn0W_H0w_t0_c00K_n0W}</div>";
    }
  }
  ?>

</body>
</html>



