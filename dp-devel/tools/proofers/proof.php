<?
$relPath="./../../pinc/";
include($relPath.'dp_main.inc');
set_magic_quotes_runtime(1);
/* $_GET $project, $proofstate, $orient, $text_data, $fileid, $imagefile, $js,
         $saved, $editone, $lang */

if (!isset($saved))
{
    //Make sure project is still available
    $sql = "SELECT * FROM projects WHERE projectid = '$project' LIMIT 1";
    $result = mysql_query($sql);
    $state = mysql_result($result, 0, "state");
    if ((($proofstate == 2) && ($state != 2)) || (($proofstate == 12) && ($state != 12))) {
  if ($userP['i_newwin']==0)
  {
  $body="No more files available for proofing for this round of the project.<BR> You will be taken back to the project page in 4 seconds.";
  metarefresh(4,"proof_per.php\" TARGET=\"_top\"",'Project Round Complete',$body);
  exit;}
  else {
  include($relPath.'doctype.inc');
  echo "$docType\r\n<HTML><HEAD><TITLE>Project Round Complete</TITLE></HEAD><BODY>";
  echo "No more files available for proofing in this round of the project.<BR>";
  echo "Please <A HREF=\"#\" onclick=\"window.close()\">click here</A> to close the proofing window.";
  echo "</BODY></HTML>";
  exit;}
    }

        $timestamp = time();
        //find page to be proofed.
          $dbQuery="SELECT fileid, image, ";
          if ($proofstate==12) {$dbQuery.="round1_text";}
          else {$dbQuery.="master_text";}
          $dbQuery.=" FROM $project WHERE state='";
          if ($proofstate==12) {$dbQuery.="12' AND round1_user != '$pguser'";}
          else {$dbQuery.="2'";}
          $dbQuery.=" ORDER BY image ASC LIMIT 1";
        $result=mysql_query($dbQuery);
        $numrows = mysql_num_rows($result);
        if ($numrows == 0) {
        if ($userP['i_newwin']==0)
          {$body="No more files available for proofing for this project.<BR> You will be taken back to the project page in 2 seconds.";
            metarefresh(2,'proof_per.php','Project Round Complete',$body);exit;}
        else {
        include($relPath.'doctype.inc');
        echo "$docType\r\n<HTML><HEAD><TITLE>Project Round Complete</TITLE></HEAD><BODY>";
        echo "No more files available for proofing in this round of the project.<BR>";
        echo "Please <A HREF=\"#\" onclick=\"window.close()\">click here</A> to close the proofing window.";
        echo "</BODY></HTML>";
        exit;}
        } else {
            $fileid = mysql_result($result, 0, "fileid");
            $imagefile = mysql_result($result, 0, "image");
            $dbQuery="UPDATE $project SET state='";
            if ($proofstate==12)
            {$oText=mysql_result($result, 0, "round1_text");}
            else {$oText=mysql_result($result, 0, "master_text");}
            if ($proofstate==12)
            {$dbQuery.="15', round2_time='$timestamp', round2_user='$pguser', round2_text='$oText'";}
            else {$dbQuery.="5', round1_time='$timestamp', round1_user='$pguser', round1_text='$oText'";}
            $dbQuery.="  WHERE fileid='$fileid' AND image='$imagefile'";
            $update = mysql_query($dbQuery);
        }

}

            $pageNum=substr($imagefile,0,-4);
            $fileid = '&fileid='.$fileid;
            $imagefile = '&imagefile='.$imagefile;
            $newproofstate = '&proofstate='.$proofstate;
// will need to add a true language option to this in future
            $lang=isset($lang)? $lang:'1';
            $lang="&lang=$lang";
$frame1=isset($saved)? 'saved':'imageframe';
            $frame1 = $frame1.'.php?project='.$project.$imagefile;
            $frame3 = 'textframe.php?project='.$project.$imagefile.$fileid.$newproofstate.$lang;
if (isset($editone)) {$editone="&editone=$editone"; $frame3.=$editone;}
if (isset($saved)) {$saved="&saved=$saved"; $frame3.=$saved;}

            include('frameset.inc');

?>
