<?PHP
$relPath="./../pinc/";
include($relPath.'html_main.inc');
include_once($relPath.'theme.inc');

$signin = _("Sign In");
$htmlC->startHeader($signin);
$htmlC->startBody(0,1,0,0);
$tb=$htmlC->startTable(0,0,0,1);
$tr=$htmlC->startTR(0,0,1);
$td1=$htmlC->startTD(2,0,2,0,"center",0,0,1);
$td2=$htmlC->startTD(1,0,0,0,"center",0,0,1);
$td3=$htmlC->startTD(0,0,0,0,"center",0,0,1);
$td4=$htmlC->startTD(1,0,2,0,"center",0,0,1);
$td5=$htmlC->startTD(0,0,2,0,"center",0,0,1);
$tde=$htmlC->closeTD(1);
$tre=$htmlC->closeTD(1).$htmlC->closeTR(1);

$destination = ( isset($_GET['destination']) ? $_GET['destination'] : '' );
echo "<form action='login.php' method='POST'>";
echo "<input TYPE='hidden' NAME='destination' VALUE='$destination'>";
echo $tb;
echo $tr.$td1;
echo "<B>"._("Distributed Proofreaders Sign In")."</B>";
echo $tre.$tr.$td2;
echo "<STRONG>"._("User Name")."</STRONG>";
echo $tde.$td3;
echo '<INPUT TYPE="text" NAME="userNM" SIZE="12" MAXSIZE="50">';
echo $tre.$tr.$td2;
echo "<STRONG>"._("Password")."</STRONG>";
echo $tde.$td3;
echo '<INPUT TYPE="password" NAME="userPW" SIZE="12" MAXSIZE="50">';
echo $tre.$tr.$td1;
echo "<INPUT TYPE="submit" VALUE='"._("Sign In")."'>";
echo $tre.$tr.$td5;
echo "<B>"._("Note").":</B>"._("User Name & Password are case sensitive.<BR>Make sure your caps lock is not on.");
echo $tre.$tr.$td4;
echo "<A HREF='addproofer.php'><B>"._("New User?")."</B></A>";
echo $tre.$htmlC->closeTable(1)."</form>".$htmlC->closeBody(1);
?>
