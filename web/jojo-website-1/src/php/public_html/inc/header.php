<div id="header">
    <a href="/"><button class="header-button">Home</button></a>
    <?php if (isset($_SESSION['username'])) {
        ?>
        <a href="/?page=profile"><button class="header-button">My profile</button></a>
        <a href="/?page=logout"><button class="header-button">Logout</button></a>
        <?php if (isset($_SESSION['admin'])) {
        ?>
        <a href="/?page=admin"><button class="header-button">Admin</button></a>
        <?php }
    } else { ?>
    <a href="/?page=login"><button class="header-button">Login</button></a>
        <?php
    } ?>
    <div id="image-header"> <img src="./images/logo_green.png" alt="algorab_logo"> </div>
</div>