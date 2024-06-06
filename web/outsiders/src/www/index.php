<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Fira+Code:wght@300..700&display=swap" rel="stylesheet">
    <title>Outsiders</title>
    <style>
    body {
        background: #000;
        font-family: 'Fira Code', monospace;
        font-optical-sizing: auto;
        padding: 2em;
        display: flex;
        align-items: center;
        justify-content: center;
        height: 50vh;
    }

    .text-slider {
        animation: slide 30s infinite;
        color: #aaa;
    }

    @media (prefers-color-scheme: dark) {
        body {
            -webkit-filter: contrast(2);
            filter: brightness(100%);
        }
    }

    @keyframes slide {
        0%   { transform: translateX(0%);  }
        50%  { transform: translateX(10vw); }
        100% { transform: translateX(0%);  }
    }
    </style>
  </head>
  <body>
    <div class="text-slider">
    <?php

        if (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
            $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
        } else {
            $ip = $_SERVER['REMOTE_ADDR'];
        }

        if ($ip == "127.0.0.1") {
            echo "<h2>I can give the flag to myself, right ? N0PS{XF0rw4Rd3D}</h2>";
        } else {
            echo "<h2>You come from the big outside, I don't trust you.</h2>";
        }

        ?>
    </div>
  </body>
</html>



