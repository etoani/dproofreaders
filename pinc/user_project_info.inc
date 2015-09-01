<?php
include_once($relPath.'pg.inc'); // get_pg_catalog_url_for_etext
include_once($relPath.'maybe_mail.inc');
include_once($relPath.'forum_interface.inc'); // get_forum_email_address()

function upi_ensure_row( $username, $projectid )
// There may not yet be a row for this ($username,$projectid) pair
// in the user_project_info table. Ensure that one exists.
{
    mysql_query("
        INSERT IGNORE INTO user_project_info
        SET
            username    = '$username',
            projectid   = '$projectid'
    ") or die(mysql_error());
    // If there's already a row for this ($username,$projectid) pair,
    // the insertion doesn't happen; it would raise a duplicate-key error,
    // except that the IGNORE causes the error to be downgraded to a warning.

    // Note that if we were assuming MySQL 4.1.0 or higher, we probably wouldn't
    // need this function.
}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

function upi_set_t_latest_home_visit( $username, $projectid, $timestamp )
{
    upi_ensure_row( $username, $projectid );

    mysql_query("
        UPDATE user_project_info
        SET t_latest_home_visit = $timestamp
        WHERE username='$username' AND projectid='$projectid'
    ") or die(mysql_error());
}

function upi_set_t_latest_page_event( $username, $projectid, $timestamp )
{
    upi_ensure_row( $username, $projectid );

    mysql_query("
        UPDATE user_project_info
        SET t_latest_page_event = $timestamp
        WHERE username='$username' AND projectid='$projectid'
    ") or die(mysql_error());
}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

$subscribable_project_events = array(
    'round_available' => _("Project becomes available in a round"),
    'round_complete'  => _("Project finishes a round"),
    'pp_enter'        => _("Project enters PP stage"),
    'sr_available'    => _("Project becomes available for smooth-reading"),
    'ppv_enter'       => _("Project enters PPV stage"),
    'posted'          => _("Project posted to Project Gutenberg"),
);

function get_label_for_event( $event )
{
    global $subscribable_project_events;
    $label = @$subscribable_project_events[$event];
    if ( is_null($label) )
    {
        die( "Unknown event: '$event'" );
    }
    return $label;
}

// -----------------------------------------------------------------------------
// "iste" = "is subscribed to event"

function subscribe_user_to_project_event( $username, $projectid, $event )
{
    set_user_project_event_subscription( $username, $projectid, $event, 1 );
}

function unsubscribe_user_from_project_event( $username, $projectid, $event )
{
    set_user_project_event_subscription( $username, $projectid, $event, 0 );
}

function set_user_project_event_subscription( $username, $projectid, $event, $bit )
{
    assert( $bit === 0 || $bit === 1 );

    $event_label = get_label_for_event( $event );
    // Just to ensure that $event is valid; we don't use $event_label.

    upi_ensure_row( $username, $projectid );

    // Now that we're sure the row exists, we can update it.
    $res = mysql_query("
        UPDATE user_project_info
        SET iste_$event = $bit
        WHERE username    = '$username'
            AND projectid = '$projectid'
    ") or die(mysql_error());

    /*
        In MySQL 4.1.0 or higher, we could replace those two queries with one:
            INSERT INTO user_project_info
            SET
                username    = '$username',
                projectid   = '$projectid',
                iste_$event = $bit
            ON DUPLICATE KEY UPDATE
                iste_$event = $bit
    */
}

function user_is_subscribed_to_project_event( $username, $projectid, $event )
{
    $event_label = get_label_for_event( $event );
    // Just to ensure that $event is valid; we don't use $event_label.

    $res = mysql_query("
        SELECT iste_$event
        FROM user_project_info
        WHERE username    = '$username'
            AND projectid = '$projectid'
    ") or die(mysql_error());
    if ( mysql_num_rows($res) == 0 )
    {
        // ($username,$projectid) pair does not appear in table.
        return 0;
    }
    else
    {
        return mysql_result($res,0);
    }
}

// -----------------------------------------------------------------------------

function notify_project_event_subscribers( $project, $event, $extras=array() )
{
    global $site_signoff;

    $event_label = get_label_for_event( $event );

    $projectid = $project->projectid;
    $title = $project->nameofwork;

    $msg_subject = "$event_label: \"$title\"";

    $msg_body = $project->email_introduction();

    switch ($event)
    {
        case 'posted':
            $pg_url = get_pg_catalog_url_for_etext( $project->postednum );
            $msg_body .=
                _("This project has been sent to Project Gutenberg and will soon be available for reading.")
                . " "
                . _("Usually, the files will be ready by the time you receive this mail; sometimes there may be a delay of a day or so.")
                . " "
                . sprintf(_("You can download the files via PG's online catalog at <%s>."), $pg_url )
                . "\n";
            break;

        case 'round_available':
            $msg_body .=
                sprintf( _("This project has become available for work in %s."), $extras['round']->id );
            break;

        case 'round_complete':
            $msg_body .=
                sprintf( _("This project has completed work in %s."), $extras['round']->id );
            break;

        case 'pp_enter':
            $msg_body .=
                _("This project has entered post-processing.");
            break;

        case 'sr_available':
            $msg_body .=
                _("This project has become available for smooth-reading.");
            break;

        case 'ppv_enter':
            $msg_body .=
                _("This project has entered post-processing verification.");
            break;

        default:
            assert(0);
    }

    $msg_body .= "\n" . $site_signoff;

    $res1 = mysql_query("
        SELECT username
        FROM user_project_info
        WHERE projectid = '$projectid'
            AND iste_$event = 1
    ") or die(mysql_error());
    $n_subscribers = mysql_num_rows($res1);
    while ( list($username) = mysql_fetch_row($res1) )
    {
        $email = get_forum_email_address($username);
        maybe_mail(
            $email,
            $msg_subject,
            sprintf( _("Hello %s,"), $username ) . "\n" . $msg_body
        );
    }

    if ( $event == 'posted' )
    {
        // Take the number of users subscribed to the event
        // and record that as the project's "interest level".
        $res4 = mysql_query("
            UPDATE projects
            SET int_level = '$n_subscribers'
            WHERE projectid = '$projectid'
        ") or die(mysql_error());
    }
}

// -----------------------------------------------------------------------------

function get_n_users_subscribed_to_events_for_project( $projectid )
// Return an array: keys are the same as $subscribable_project_events,
// each value is the number of subscriptions to that event for the given project.
{
    global $subscribable_project_events;
    $items = array();
    foreach ( $subscribable_project_events as $event => $label )
    {
        $items[] = "SUM(iste_$event) AS $event";
    }
    $selects = implode( ',', $items );

    $res = mysql_query("
        SELECT $selects
        FROM user_project_info
        WHERE projectid='$projectid'
    ") or die(mysql_error());
    $r = mysql_fetch_assoc($res);

    // If there are no rows for this project, the SUMs will all return NULL;
    // change those to zeroes.
    $r = array_map( 'intval', $r );

    return $r;
}

function create_temporary_project_event_subscription_summary_table()
// Create a temp table named 'project_event_subscriptions_grouped_by_project'
// with two columns: 'projectid' and 'nouste_posted', where the latter gives the
// current number of users that are subscribed to the project's 'posted' event.
// ("nouste" = "number of users subscribed to event")
{
    mysql_query("
        CREATE TEMPORARY TABLE project_event_subscriptions_grouped_by_project
        (
            PRIMARY KEY (projectid)
        )
        SELECT
            projectid,
            COUNT(*) AS nouste_posted
        FROM user_project_info
        WHERE iste_posted = 1
        GROUP BY projectid
    ") or die(mysql_error());
}

// -----------------------------------------------------------------------------

/*
A better (or at least, more flexible) way to implement event subscriptions
would be something like the following...

Currently we have various scattered calls to notify_project_event_subscribers().
Instead, push them all down into log_project_event() (thus subjecting *all*
events to an automatic subscription-check).

Currently we predefine a set of subscribable event-codes and a corresponding
set of columns in the user_project_info table, and each call to
notify_project_event_subscribers() specifies one of those event-codes,
which (along with the projectid) determines which subscriptions are triggered.
Instead, give notify_project_event_subscribers() access to all data passed to
log_project_event(), and have each subscription specify "patterns" in that
data that trigger that subscription.

For example, rather than having a 'round_available' event-code, a comparable
subscription would look for project-events in which
    event_type was 'transition',
    details1 was the 'proj_waiting' state for some round, and
    details2 was the 'proj_avail' state for the same round.

Rather than forcing each subscription to be attached to a particular project,
it would be possible to allow pattern-matching on the project. E.g. a user could
be notified when any book of a specified genre is available for P1 proofreading
(Task 16) or smooth-reading (Task 677).

The main problem with this scheme is specifying and implementing the event
pattern-matching mechanism, and efficiently finding all the subscriptions that
match a given event.
*/

// vim: sw=4 ts=4 expandtab
?>