<?php
   echo "<!DOCTYPE html>";
   echo "<html>";
   echo "<head>";
   echo "<meta charset=\"UTF-8\">";
   echo "</head>";
   echo "<body>";
   echo "<h1>You have been successful</h2>";
   $fn = trim($_POST['first_name']);
   $ln = trim($_POST['last_name']);
   echo "<p>Thank you for using our CGI " . $fn . " " .  $ln . "!!!!!! </p>";
   echo "</body>";
   echo "</html>";
?>