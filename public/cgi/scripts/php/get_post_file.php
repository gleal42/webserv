#!/usr/bin/php
<?php
echo "<!DOCTYPE html>";
echo "<html>";
echo "<head>";
echo "<meta charset=\"UTF-8\">";
echo "</head>";
echo "<body>";
$method = trim($_SERVER['REQUEST_METHOD']);
if ($_SERVER['REQUEST_METHOD'] === 'POST')
{
   if (upload_file() == false)
   {
      http_response_code(400);
      return ;
   }
   thank_user();
}
else if ($_SERVER['REQUEST_METHOD'] === 'GET'){
   echo "<h1> List of files </h1>";
   $path = "../../../uploads/";
   $files = scandir($path);
   $files = array_diff(scandir($path), array('.', '..'));
   $pos_public = strpos($path, "public/");  // temporary so that public logic is implemented logic
   if ($pos_public != false)
   {
      $start_path = substr($path, 0, $pos_public);
      $pos_public = $pos_public + strlen("public/");
      $path = substr($path, $pos_public);
      $path = $start_path . $path;
   }
   foreach ($files as $value) {
      echo "Path is $path\n";
      echo "Value is $value\n";
      echo "<p><a href=\"" . $path.$value . "\">" . $value . "</a><p>";
   }
}
else{
   http_response_code(501);
   echo "<h2>Invalid Request</h2>";
   echo "</body>";
   echo "</html>";
   return;
}
echo "<h1>You have been successful in the ".$method." request</h2>";
echo "</body>";
echo "</html>";

// Upload dir will come as environment variable?

function upload_file()
{
   $upload_dir = "../../../uploads/";
   $filename = trim($_FILES["cute_picture"]["name"]);
   $file_to_upload = basename($filename);
   $extension = pathinfo($file_to_upload, PATHINFO_EXTENSION);
   $file_to_upload = pathinfo($file_to_upload, PATHINFO_FILENAME);
   $temp = $upload_dir . $file_to_upload . "." . $extension;
   $nbr = 1;
   while (file_exists($temp))
   {
      $temp = $upload_dir . $file_to_upload . $nbr . "." . $extension;
      $nbr = $nbr + 1;
   }
   $file_to_upload = $temp;
   if (move_uploaded_file($_FILES["cute_picture"]["tmp_name"], $file_to_upload)) {
      echo "<p>The file ". basename(htmlspecialchars( $file_to_upload )). " has been uploaded.</p>";
   } else {
      echo "Sorry, there was an error uploading your file.";
      echo "Not uploaded because of error #".$_FILES["cute_picture"]["error"];
   }
   return (true);

}

function thank_user()
{
   $intro = "";
   if (isset($_POST['gender']))
   {
      $gender = set_name_separator($_POST['gender'], "");
      $intro = appropriate_intro($gender);
   }
   $fn = set_name_separator($_POST['first_name'], " ");
   $ln = set_name_separator($_POST['last_name'], " ");
   $name = $fn.$ln;
   if (empty($fn) && empty($ln)) {
      $name = "whoever you are";
   }
   echo "<p>Thank you for using our CGI, ".$intro.$name."!!!!!! </p>";
}

function set_name_separator( $name , $separator)
{
   if (isset($name) && !empty($name)) {
      $name = $separator.trim($name);
   } else {
      $name="";
   }
   return $name;
}

function appropriate_intro( $gender )
{

   if ($gender == "male") {
      $intro = "Mr.";
   } else if ($gender == "female") {
      $intro = "Mrs.";
   } else {
      $intro = "";
   }
   return $intro;
}

?>
