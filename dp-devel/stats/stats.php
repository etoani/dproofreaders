<?
$relPath='./../pinc/';
include($relPath.'v_site.inc');
include($relPath.'project_states.inc');
include($relPath.'connect.inc');
$db_Connection=new dbConnect();

$stats_inc_path="$dynstats_dir/stats.inc";

if (!file_exists($stats_inc_path)) {
	touch($stats_inc_path);
	$statsfile = fopen($stats_inc_path, "w");
	fputs($statsfile, "<?\n");
	fputs($statsfile, "\$sitestats['monthly'] = \"0\";\n");
	fputs($statsfile, "\$sitestats['daily'] = \"0\";\n");
	fputs($statsfile, "\$sitestats['goal'] = \"0\";\n");
	fputs($statsfile, "?>\n");
	}

$today = getdate();
$midnight = mktime(0,0,0,$today['mon'],$today['mday'],$today['year']);

//limit to looking at projects which do not have
//the archive flag set to 1 in order to limit run time
$allProjects = mysql_query("SELECT projectid FROM projects WHERE archived ='0' AND state != '".PROJ_PROOF_FIRST_WAITING_FOR_RELEASE."'");
$numProjects = mysql_num_rows($allProjects);

while ($i < $numProjects) {
        $projectID = mysql_result($allProjects, $i, "projectid");

        $result1 = mysql_query("SELECT COUNT(*) FROM $projectID WHERE
           round1_time >= $midnight AND
           (state='save_first' OR state LIKE '%_second%')");


        $result2 = mysql_query("SELECT COUNT(*) FROM $projectID WHERE
           state='save_second' AND round2_time >= $midnight");

        $row1 = mysql_fetch_row($result1);
        $pages1 = $row1[0];

        $row2 = mysql_fetch_row($result2);
        $pages2 = $row2[0];

        $rows = $pages1+$pages2;

        $dailyPages = $dailyPages+$rows;
        $i++;
}

//echo result so we know cron job is working and to avoid timeout
echo "Daily pages: ".number_format($dailyPages);

$result = mysql_query("SELECT SUM(pages) AS monthlypages FROM pagestats WHERE month=".$today['mon']." AND year=".$today['year']."");
$monthlyPages = mysql_result($result, 0, "monthlypages");

//Let's only update the monthly goal if it is the first day of the month and it is
//between 0000 hours and 0300 hours (to allow for cron job delays).  Always update the monthly
//goal if override_goal has been set
if (($today['mday'] == 1 && ($today['hours'] >= 0 && $today['hours'] <= 3)) || $_GET['override_goal'] == 1) {
$result = mysql_query("SELECT SUM(dailygoal) AS monthlygoal FROM pagestats WHERE year=".$today['year']." AND month=".$today['mon']."");
$monthlyGoal = mysql_result($result, 0, "monthlygoal");
$updateMonthlyGoal = 1;
}

//Read the entire stats file into the $lines array
$i=0;
$lines = file($stats_inc_path);
$statsfile = fopen($stats_inc_path, "w");
while ($i < count($lines)) {
	if (substr($lines[$i], 12, 7) == "monthly") { fputs($statsfile, "\$sitestats['monthly'] = \"$monthlyPages\";\n"); }
	elseif (substr($lines[$i], 12, 5) == "daily") {	fputs($statsfile, "\$sitestats['daily'] = \"$dailyPages\";\n");	}
	elseif (substr($lines[$i], 12, 4) == "goal" && $updateMonthlyGoal == 1) { fputs($statsfile, "\$sitestats['goal'] = \"$monthlyGoal\";\n");	}
	else { fputs($statsfile, trim($lines[$i])."\n"); }
	$i++;
	}
	fclose($statsfile);

?>

