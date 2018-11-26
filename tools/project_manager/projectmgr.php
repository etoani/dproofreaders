<?php
$relPath="./../../pinc/";
include_once($relPath.'base.inc');
include_once($relPath.'user_is.inc');
include_once($relPath.'theme.inc');
include_once($relPath.'metarefresh.inc');
include_once($relPath.'misc.inc'); // array_get()
include_once($relPath.'SettingsClass.inc');
include_once($relPath.'special_colors.inc');
include_once($relPath.'gradual.inc');
include_once($relPath.'ProjectSearchForm.inc');
include_once($relPath.'ProjectSearchResults.inc');
include_once($relPath.'site_news.inc');
include_once('projectmgr.inc'); // echo_manager_links();

require_login();

switch ($userP['i_pmdefault'])
{
    case 0:
        $default_view = "user_all";
        break;
    case 1:
        $default_view = "user_active";
        break;
    default:
        $default_view = "blank";
}

try {
    $show_view = get_enumerated_param($_GET, 'show', $default_view,
        array('search_form', 'search', 'user_all', 'user_active',
              'blank', 'set_columns', 'config'));
} catch(Exception $e) {
    $show_view = 'blank';
}

if(!user_is_PM())
{
    // Redirect to the new search page for bookmarked URLs
    $url = "$code_url/tools/search.php";
    if($show_view == "search")
    {
        // pull out everything after the ? and redirect
        $url .= substr($_SERVER['REQUEST_URI'], stripos($_SERVER['REQUEST_URI'], '?'));
    }
    metarefresh(0, $url);
}

// exits if handled
handle_set_cols($show_view, "PM");

$header_args = array(
    "js_files" => array("$code_url/tools/dropdown.js"));

output_header(_("Project Management"), NO_STATSBAR, $header_args);

// exits if handled
handle_config($show_view, "PM", _("Configure Project Management Page"));

$PROJECT_IS_ACTIVE_sql = "(state NOT IN ('".PROJ_SUBMIT_PG_POSTED."','".PROJ_DELETE."'))";

if ($show_view == 'search_form')
{
    $search_form = new ProjectSearchForm();
    echo "<h1>", _("Project Management"), "</h1>";
    $search_form->render();
    exit();
}

if($show_view == "blank")
{
    // TRANSLATORS: Abbreviation for Project Manager
    $sub_title = sprintf(_("PM: %s"), $pguser);
}
elseif($show_view == "user_all")
{
    $condition = "username = '$pguser'";
    // adjust $_GET so will work corectly with refine search and sort and navigate
    // keep "user_all" or we won't know it is user all
    $_GET = array_merge($_GET, array('project_manager' => $pguser));
    // TRANSLATORS: Abbreviation for Project Manager
    $sub_title = sprintf(_("All PM projects: %s"), $pguser);
}
elseif ($show_view == "user_active")
{
    $condition = "$PROJECT_IS_ACTIVE_sql AND username = '$pguser'";
    $_GET = array_merge($_GET, array(
        'project_manager' => $pguser,
        'state' => array_diff($PROJECT_STATES_IN_ORDER, array(PROJ_SUBMIT_PG_POSTED, PROJ_DELETE))
    ));
    // TRANSLATORS: Abbreviation for Project Manager
    $sub_title = sprintf(_("Active PM projects: %s"), $pguser);
}
else // $show_view == 'search'
{
    $search_form = new ProjectSearchForm();
    $condition = $search_form->get_condition();
    $sub_title = _("Search Results");
}

$search_results = new ProjectSearchResults("PM");

echo_manager_links();
echo "<h1>", _("Project Management"), "</h1>\n";
// possibly show message, but don't exit
check_user_can_load_projects(false);
show_news_for_page('PM');
echo "\n<h2>$sub_title</h2>\n";
echo_shortcut_links($show_view);

if($show_view == "blank")
    exit();

$search_results->render($condition);

function create_shortcut_link($text, $show_val, $show_view="")
{
    if($show_view != $show_val)
        return "<a href='?show=$show_val'>$text</a>";
    else
        return $text;
}

function echo_shortcut_links($show_view)
{
    $links = array(
        // TRANSLATORS: Abbreviation for Project Manager
        create_shortcut_link(_("View your Active PM Projects"), "user_active", $show_view),
        // TRANSLATORS: Abbreviation for Project Manager
        create_shortcut_link(_("View All your PM Projects"), "user_all", $show_view),
    );

    if($show_view != "blank")
        $links[] = get_refine_search_link();

    $links[] = get_search_configure_link();

    echo implode(" | ", $links);
}

// vim: sw=4 ts=4 expandtab
