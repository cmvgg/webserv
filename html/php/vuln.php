<?php

$page = $_GET['page'];
if(isset($page))
    include $page;
else
    include '../403.html';

?>