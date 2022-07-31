<?php
   echo "<!DOCTYPE html>";
   echo "<html>";
   echo "<head>";
   echo "<meta charset=\"UTF-8\">";
   echo "</head>";
   echo "<body>";
   echo "<h1>You have been successful</h2>";
   echo "<p>Thank you for using our CGI " . $_POST['first_name'] . " " .  $_POST['last_name'] . "!!!!!!</p>";
   echo "</body>";
   echo "</html>";
?>