<?php
echo "<!DOCTYPE html>";
echo "<html>";
echo "<head>";
echo "<meta charset=\"UTF-8\">";
echo "</head>";
echo "<body>";
$method = trim($_SERVER['REQUEST_METHOD']);
echo "<h1>You have been successful in the ".$method." request</h2>";
if ($_SERVER['REQUEST_METHOD'] === 'POST')
{
   if (isset($_POST['gender']) && !empty($_POST['gender'])) {
      $gender = trim($_POST['gender']);
   } else {
      $gender = "";
   }

   if ($gender == "male") {
      $intro = "Mr.";
   } else if ($gender == "female") {
      $intro = "Mrs.";
   } else {
      $intro = "";
   }

   $fn = set_name($_POST['first_name']);
   $ln = set_name($_POST['last_name']);
   $name = $fn.$ln;
   if (empty($fn) && empty($ln)) {
      $name = "whoever you are";
   }

   echo "<p>Thank you for using our CGI, ".$intro.$name."!!!!!! </p>";
}
else {
   echo "<h1>STILL IMPLEMENTING GET</h2>";
}
echo "</body>";
echo "</html>";

function set_name( $name )
{
   if (isset($name) && !empty($name)) {
      $name = trim($name);
      $name=" ".$name;
   } else {
      $name="";
   }
   return $name;
}

?>