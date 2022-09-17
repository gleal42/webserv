<!DOCTYPE html>
<html>
<body>

<h1>Our PHP page</h1>

<?php

echo "Hello World!\n";
echo 'Show uploads while being respectful ' . htmlspecialchars($_GET["show_uploads"]) . '!';

$env_array =getenv();

echo "<h3>Environment variables :</h3>";

foreach ($env_array as $key=>$value)

{
    echo "$key => $value <br />";
}

// Get variables
echo "<h3>GET variables:</h3>";
echo "<pre>";
print_r($_GET);
echo "</pre>";
echo "<br />";

// Post variables
echo "<h3>POST variables:</h3>";
echo "<pre>";
print_r($_POST);
echo "</pre>";
echo "<br />";

?>

</body>
</html>