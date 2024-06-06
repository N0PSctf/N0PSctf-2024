<?php

    session_start();

    define("INC_FOLDER", "/var/www/html/inc/");

    $redis = new Redis();
    $redis->connect('redis', 6379);

    $request_limit = 10;
    $time_limit = 5;
    $total_requests = 0;
    $timeout = 60;

    $user_ip_address = $_SERVER['REMOTE_ADDR'];

    if (!$redis->exists($user_ip_address)) {
        $redis->set($user_ip_address, 1);
        $redis->expire($user_ip_address, $time_limit);
    } else {
        $total_requests = $redis->get($user_ip_address);
        if ($total_requests < 0) {
            echo "<pre>Stop spamming my website, you hacker! You got time out!</pre>";
            die();
        }
        $redis->INCR($user_ip_address);
        $total_requests = $redis->get($user_ip_address);
        if ($total_requests > $request_limit) {
            $redis->set($user_ip_address, -1);
            $redis->expire($user_ip_address, $timeout);
            echo "<pre>Stop spamming my website, you hacker! You got time out!</pre>";
            die();
        }
    }

    function check_logged_in() {
        if (!isset($_SESSION['username'])) {
            header("Location: /?page=login");
            die();
        }
    }

    function check_admin() {
        check_logged_in();
        if (!isset($_SESSION['admin'])) {
            header("Location: /");
            die();
        }
    }

    function login() {
        include(INC_FOLDER . 'login.php');
    }

    function register() {
        include(INC_FOLDER . 'register.php');
    }

    function logout() {
        check_logged_in();
        include(INC_FOLDER . 'logout.php');
    }

    function profile() {
        check_logged_in();
        include(INC_FOLDER . 'profile.php');
    }

    function admin() {
        check_admin();
        include(INC_FOLDER . 'admin.php');
    }

    function home() {
        check_logged_in();
        include(INC_FOLDER. 'home.php');
    }

    $page = '';

    if (isset($_GET['page'])) {
        $page = $_GET['page'];
    }

    match ($page) {
        'login' => login(),
        'register' => register(),
        'logout' => logout(),
        'profile' => profile(),
        'admin' => admin(),
        default => home(),
    };

?>