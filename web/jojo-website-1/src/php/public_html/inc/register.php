<?php
    require(INC_FOLDER . "config.php");
    $already_exists = false;
    $successful = false;
    $sql_err = false;

    if (isset($_POST['username'], $_POST['password'])) {
        if ($stmt = $conn->prepare('SELECT id FROM users WHERE username = ?')) {
            $stmt->bind_param('s', $_POST['username']);
            $stmt->execute();
            $stmt->store_result();
            if ($stmt->num_rows > 0) {
                $already_exists = true;             
            } else {
                if ($stmt = $conn->prepare('INSERT INTO users (username, password) VALUES (?, ?)')) {
                    $password = password_hash($_POST['password'], PASSWORD_BCRYPT);
                    $stmt->bind_param('ss', $_POST['username'], $password);
                    $stmt->execute();
                    header('Location: /?page=login');
                } else {
                    $sql_err = true;             
                }
            }
            $stmt->close();
        } else {
            $sql_err = true;
        }
        $conn->close();
    }
?>

<!DOCTYPE html>
<html>
	<head>
        <link rel="stylesheet" type="text/css" href="https://fonts.googleapis.com/css?family=Inconsolata"/>
        <link rel="icon" href="./images/favicon.ico" type="image/x-icon">
        <link rel="stylesheet" href="./styles/global.css">
		<meta charset="utf-8">
		<title>Register</title>
	</head>
	<body>
    <?php include(INC_FOLDER . "header.php"); ?>
    <div id="message-notif" <?php if (!($already_exists) && !($sql_err)) echo 'style="display:none;"'; ?>>
    <?php
        if ($already_exists) echo 'This username is already taken, please choose another!';
        elseif ($sql_err) echo 'Could not prepare statement!';
    ?>
</div>
        <section class="background-section">
            <?php include(INC_FOLDER . "background.php"); ?>

    </section>
    <section>
            <div>
                <h1>Register</h1>
                <form action="/?page=register" method="post">
                    <div class="input">
                        <label for="username">Username:</label>
                        <input type="text" name="username" id="username" required>
                    </div>
                    <div class="input">
                        <label for="password">Password:</label>
                        <input type="password" name="password" id="password" required>
                    </div>
                    
                    <input type="submit" value="Register">
                    
                </form>
            </div>
        </section>
	</body>
</html>