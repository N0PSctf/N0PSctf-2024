<?php
    require(INC_FOLDER . "config.php");

    $already_exists = false;
    $successful = false;
    $sql_err = false;

    $change_username = false;

    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        $id = $_POST['id'];
        $username = $_POST['username'];
        if (isset($_POST["username"])) {
            $username = $_POST["username"];
        }
        if (isset($_POST["password"])) {
            $password = password_hash($_POST["password"], PASSWORD_BCRYPT);
        }
        if ($stmt = $conn->prepare('UPDATE users SET username = ?, password = ? WHERE id = ?')) {
            $stmt->bind_param('ssi', $username, $password, $id);
            $stmt->execute();
            $successful = true;
        } else {
            $sql_err = true;
        }
        $stmt->close();
        }

?>

<!DOCTYPE html>
<html>
	<head>
        <link rel="stylesheet" type="text/css" href="https://fonts.googleapis.com/css?family=Inconsolata"/>
        <link rel="icon" href="./images/favicon.ico" type="image/x-icon">
        <link rel="stylesheet" href="./styles/global.css">
		<meta charset="utf-8">
		<title>Profile</title>
	</head>
	<body>
        <?php include(INC_FOLDER . "header.php"); ?>
        <div id="message-notif" <?php if (!($sql_err) && !($successful)) echo 'style="display:none;"'; ?>>
            <?php
                if ($sql_err) echo 'Could not prepare statement!';
                elseif ($successful) echo 'Account successfully updated!';
            ?>
        </div>
        <section class="background-section">
            <?php include(INC_FOLDER . "background.php"); ?>
        </section>
        <section>
            <div>
                <h1>Manage your profile</h1>
                <form action="/?page=profile" method="post">
                    <input type="hidden" name="id" value="<?php echo $_SESSION['id'];?>">
                    <div class="input">
                        <label for="username">Username:</label>
                        <input type="text" name="username" id="username" value="<?php echo htmlentities($_SESSION['username']); ?>" required>
                    </div>
                    <div class="input">
                        <label for="password">Password:</label>
                        <input type="password" name="password" id="password" required>
                    </div>
                    <input type="submit" value="Update">
                </form>
            </div>
        </section>
	</body>
</html>