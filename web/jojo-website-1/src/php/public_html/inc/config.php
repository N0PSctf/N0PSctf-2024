<?php

$mysql_pass = "6bba23da7f6df87eb215120d86e8ebd8001a695dabafcf8d6ac4b437ab06b716";
$mysql_user = "root";
$mysql_host = "mysql";
$mysql_db = 'chall_db';

try {
    $conn = new mysqli($mysql_host, $mysql_user, $mysql_pass, $mysql_db);

    if ($conn->connect_error) {
        die("<h3>Environment is still loading...</h3>");
    }
} catch(Exception $e) {
    die("<h3>Environment is still loading...</h3>");
}


?>