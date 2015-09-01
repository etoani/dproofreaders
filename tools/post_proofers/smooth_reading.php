<?php
// Give information on smooth reading
// including (most importantly) the list of projects currently available

$relPath='../../pinc/';
include_once($relPath.'base.inc');
include_once($relPath.'theme.inc');
include_once($relPath.'site_news.inc');
include_once($relPath.'showavailablebooks.inc');

// ---------------------------------------
//Page construction varies with whether the user is logged in or out
if (isset($GLOBALS['pguser'])) { $logged_in = TRUE;} else { $logged_in = FALSE;}


if ($logged_in) {
    $header_text = _("Smooth Reading Pool");
    $news = "SR";
} else {
    $header_text = _("Smooth Reading Pool Preview");
    $news = "SR_PREV";
}

// we show more columns when user is logged in, so we don't have room for the stats bar
output_header( $header_text, $logged_in ? NO_STATSBAR : SHOW_STATSBAR);
$stage = get_Stage_for_id("SR");
$stage->page_header( $header_text );
show_news_for_page($news);

echo "<h3>" . _("Smooth Reading") . "</h3>";

if (!$logged_in)
{

    echo  "<p style='font-size: 120%;'>" . _("This Preview page shows which books are currently available for Smooth Reading. Click on a book's title to view more information about it or to download the text.") . "</p>";

    echo "<p>" . _("Please note that while unregistered guests are welcome to download texts for Smooth Reading, only registered volunteers are able to upload annotated texts. A registration link is available at the top of this page.") . "</p>";

}

{

    echo "<p style='font-size: 120%;'>" . _("The goal of Smooth Reading is to read the text attentively, as for pleasure, with just a little more attention than usual to punctuation, etc. This is NOT full scale proofreading, and comparison with the scans is not needed. Just read it as your normal, sensitized-to-proofreading-errors self, and report any problem that disrupts the sense or the flow of the book. Note that some of these will be due to the author and/or publisher.") . "</p>";

    echo "<p>" . _("Errors are reported by adding a comment of the form <blockquote style='color: red; background-color: inherit;'> [**correction or query] <br> </blockquote> immediately after the problem spot. Do not correct or change the problem, just note it in the above format.") . "</p>";

    echo "<h3>" . _("Examples:") . "</h3>";
    echo "<p><ul>";
    echo "<li>" . _("that was the end,[**.] However, the next day") . "</li>";
    echo "<li>" . _("that was the end[**.] However, the next day") . "</li>";
    echo "<li>" . _("that was the emd.[**end] However, the next day") . "</li>";
    echo "</ul></p>";

}

show_projects_for_smooth_reading();

// vim: sw=4 ts=4 expandtab