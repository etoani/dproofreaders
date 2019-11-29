# Installation Instructions

This file contains instructions for installing/upgrading the Distributed
Proofreaders website code. If you are upgrading an existing installation
see [UPGRADE.md](UPGRADE.md).

The instructions in this file assume that you're using a release tarball from
[DP's Project Page on github.com](https://github.com/DistributedProofreaders/dproofreaders).
If you're installing/upgrading from source control, it's trickier.

In case of problems, please post to the [DP Site Code](http://www.pgdp.net/phpBB3/viewforum.php?f=32)
forum at the [primary DP site](https://www.pgdp.net/c).

## Middleware version requirements
The following lists supported versions for the four primary middleware
components.

### PHP
PHP version 7.0 is the minimum supported version. 7.1 and later should work
but have not been tested.

The following PHP extensions are required. They are listed below with their
Ubuntu system package names.
* GD - php-gd
* Internationalization - php-intl
* MySQL - php-mysql
* zip - php-zip

### MySQL
MySQL version 5.5 or later is recommended.
Versions below 5.5, down to 5.1, may work but are untested.

MariaDB version 5.5 and later should also work but has not been tested.

### phpBB
[phpBB](https://www.phpbb.com) 3.1 and 3.2 are supported. Version 3.2 is
recommended.

For phpBB to work with the DP code, you must disable superglobal checking.
In the phpBB code edit:

* 3.1: `config/parameters.yml`
* 3.2: `config/default/container/parameters.yml`

change `core.disable_super_globals` to `false`, and flush the phpbb cache.

### jpgraph
[jpgraph](http://jpgraph.net) versions 3.5.0b1, 4.0.2, 4.1.0, and 4.2.0 will
work after applying the included patches (4.0.2 patch will work with 4.1.0
and 4.2.0). Version 4.2.0 is recommended.

## Distro support
These middleware components match the following major distribution releases:
* Ubuntu 14.04, Trusty (with PHP 7.0 upgrade)
* Ubuntu 16.04, Xenial
* Ubuntu 18.04, Bionic
* RHEL / CentOS 6.x family (with PHP 7.0 upgrade)
* RHEL / CentOS 7.x family (with PHP 7.0 upgrade)

## Installing from scratch
This section assumes that you don't have an existing installation of DP.
If you're upgrading from a previous version, see [UPGRADE.md](UPGRADE.md).

When we say to install and/or configure some third-party software, that's
assuming it isn't already.

### Install a web server
Any web server that supports PHP should work, although all testing has
been done using Apache 2. All known deployments, including pgdp.net,
are using Apache 2 as well.

See `apache2.conf.example` for an example Apache config file, including
examples on enabling page compression and caching.

### Install PHP
* Enable GD (for jpgraph) if you want dynamically-generated graphs.
* Enable YAZ if you want to be able to do external catalog searches when
  creating projects.
* Enable the CLI for running cron jobs and future upgrade scripts.
* Configure the PHP [opcode cache](https://www.php.net/manual/en/opcache.configuration.php)
  to improve site performance. Allocate 128MB to the cache if possible.

Consider the following settings in `php.ini`:
* `session.gc_probability = 1` (or something other than zero)
  Needed to trigger garbage collection in 'sessions' table
  if `_USE_PHP_SESSIONS=TRUE` in your DP config file.
* `magic_quotes_gpc`, `register_globals`, `register_long_arrays` - 
  The DP codes does not rely on these features and they can
  be turned off if no other code on the server uses them.

### Install MySQL
In the examples in this document, we will assume that the MySQL server
is on "localhost", i.e. the same host on which you are running these
commands, and the same host that runs the webserver.

### Install gettext and xgettext
If you want to localize the site messages, install gettext and xgettext.

### Install jpgraph
The statistics code depends on jpgraph for graph generation.
Follow the installation instructions included with jpgraph.
If using version 4.0.2 or 4.2.0, apply `jpgraph-4.0.2.patch`.
If using version 3.5.0b1, apply `jpgraph-3.5.0b1.patch`.

The simplest place to install jpgraph is in your document root.
You will need the location of the jpgraph installation to put into the
site-configuration for the DP code.

The DP code can take advantage of jpgraph caching if it is
enabled. See jpgraph documentation on how to enable the cache.

### Install aspell
The code uses aspell for the spellchecker in the proofreading interface.
Note that for aspell 0.50 you may have to symlink the .dat files due to
some filename inconsistencies. For example, iso8859-1.dat should have a
symlink iso-8859-1.dat pointing to it, and similar for all other dictionaries.

This is not required for aspell 0.60 or higher.
You need aspell 0.60 if you want to support UTF-8.

### Install wdiff
WordCheck uses [wdiff](http://www.gnu.org/software/wdiff/wdiff.html)
to assist Project Managers in detecting stealth scannos ("Suggestions from
diff analysis" in `c/tools/project_manager/show_project_stealth_scannos.php`).
If wdiff is not installed this one tool will fail but the rest of
WordCheck will operate correctly.

### Install WikiHiero (optional)
Install it somewhere in server's document hierarchy.

For example:
```bash
wget http://aoineko.free.fr/wikihiero.zip
unzip -d wikihiero wikihiero.zip
rm wikihiero.zip

vim wikihiero/wikihiero.php
# On line 59, for definition of WH_IMG_DIR,
# change "$wgScriptPath/extensions/wikihiero/img/"
# to just "img/"

vim wikihiero/wh_language.php
# The file begins with three bytes (EF BB BF), which
# constitute the Unicode Byte Order Mark encoded in UTF-8.
# Delete those bytes?
```

### Configure MySQL
Choose names for various MySQL items:
* the DP database
* the DP archive database (to house DP data of finished projects)
* the DP user (to handle all DP and phpBB queries)
* the DP user's password

Don't pick a password that contains an apostrophe (single-quote),
as it confuses the code.

In the examples in this document, we will use the following:
* `dp_db`
* `dp_archive`
* `dp_user`
* `dp_password`

You may wish to choose names that are harder for others to guess
(especially for the password)!

Set up MySQL to create the database and user.
There are various ways to do each of these.
We'll show how to do it using the MySQL client.

Connect to the MySQL server as the root user, or any sufficiently
powerful user.
```bash
mysql -h localhost -u root -p
```

Create the database.
```
CREATE DATABASE dp_db CHARACTER SET LATIN1;
```

Create the user. (See MySQL Manual 5.5.4 Adding New Users to MySQL.)
```
GRANT ALL  ON dp_db.* TO dp_user@localhost IDENTIFIED BY 'dp_password';
```

Similarly for the archive database (if you want one):
```
CREATE DATABASE dp_archive CHARACTER SET LATIN1;
GRANT ALL  ON dp_archive.* TO dp_user@localhost IDENTIFIED BY 'dp_password';
```

Exit from the MySQL client.
```
quit;
```

### Install and configure phpBB
phpBB is most easily installed in your document root as `phpbb/`
The phpBB tables need to be installed in the same database as the DP
tables, or be in a separate database in the same MySQL instance as the
DP tables with the same authentication information.

See the "Post installation / conversion steps" section in
`phpbb3-conversion.txt` for recommended phpBB3 configuration settings
and further steps to ensure good integration with the DP code.

### Create phpBB categories and forums
When the site is operational, each team and each project will get a
discussion topic created by the code. You should create a forum to
house the team topics, and four forums for project topics, one for each
of:
* projects not yet released for proofreading
* projects released for proofreading
* projects in post-processing
* projects that have been posted

The code also assumes the existence of a few more forums, not in the
operational sense above, but just to provides helpful links.
* general
* beginners' site Q&A
* beginners' proofreading Q&A
* content providers
* post-processing

When you create these forums, you can give them any name or description
you want. After, take note of their ids -- you'll need these when you
configure the DP code.

Other than that, you can create whatever categories/forums/topics you
like. As a suggestion only, here are some of the categories and forums
at www.pgdp.net (the ones referred to above are prefixed with `**`):

    For Beginners
        ** Common Site Q&A
        ** Common Proofreading Q&A
    Site
        ** General
        -- Future Features
        -- Promotion
        -- Documentation
        -- Site-Related Polls
    Activities
        ** Providing Content
        -- Managing Projects
        -- Mentoring
        ** Post-Processing
    Projects
        ** Projects Waiting
        ** Projects Being Proofread or Formatted
        ** Projects Being Post-Processed or Verified
        -- UberProjects
        ** Archive of Posted Projects
    Community
        -- DP Culture and History
        ** Team Talk
        -- Fun Polls
        -- Project Gutenberg
        -- Everything Else (except DP)
    Helpful Software
        -- Windows
        -- Mac
        -- *nix
        -- Cross-Platform
        -- Tool Development
        -- DP Site Code

### Unpack the DP code
Somewhere within your web server's document hierarchy (possibly,
though not necessarily, in the document root) unpack the DP code.
This expands into a directory named 'c'.

### Create other directories
Create some additional directories under your web server's document
hierarchy:

    projects

    d
    d/locale
    d/stats
    d/stats/automodify_logs
    d/teams
    d/teams/avatar
    d/teams/icon
    d/pg
    d/xmlfeeds

And create the following temporary directory to house WordCheck temp files:

    /tmp/sp_check

These should have permissions `drwxrwsr-x` and have the owner & group
of the user that the DP code runs as (probably 'nobody' or 'www-data').

### Create the directory for project file uploads
In order to populate projects with images and initial text, the images
and text must reside on the server. This can be done by users via the
web with the built-in `remote_file_manager.php`, or via FTP/scp, or
both. In either case, a directory outside the web space, but writeable
by the web server, is required.

If using `remote_file_manager.php`, you can create this directory wherever
it makes sense to do so, although for security reasons it is strongly
recommended that it be outside of your web space. Ensure the directory
permissions and/or ownership allow the web server full access to the
directory and its contents.

If using FTP or scp, create a system user and have the uploads
directory located inside that user's home directory. Ensure that
new files and directories within this space have permissions set
to 777 so that the web server has full access to them.

See the 'Uploading and Creating Projects' section of `configuration.sh`
for more details.

### Configure the DP code (with site-specific settings)
Make an editable copy of `c/SETUP/configuration.sh`, and put it outside
your webserver's document hierarchy.

Modify your copy of `configuration.sh` as appropriate. Details about each
parameter are included in `configuration.sh`. Run:
```bash
c/SETUP/configure path-to-modified-configuration.sh path-to-code-dir
```
The configure script will use your modified `configuration.sh` to update
the DP code with your site-specific settings.

### Create the tables of the DP database
```bash
cd c/SETUP
php -f install_db.php
```

### Move SETUP directory outside your webserver's document hierarchy
The `c/SETUP/` directory is only needed for site configuration. **As a
security measure, move it out of your webserver's document hierarchy.**
Don't delete it as you may want to use its contents again to
reconfigure the site.

### At this point, your DP site should be functional
Try visiting the site in your browser and registering as a new user
to ensure that works.

You may want to redirect/rewrite requests for foo/ to foo/c/.
You could just create a foo/index.html that redirects.

### Install MediaWiki (optional)
None of the DP code relies on the presence of a wiki, MediaWiki or
otherwise, but most DP sites have some form of wiki for user
contributions, coordination, documentation, etc. If you have a wiki,
set `_WIKI_URL` in `configuration.sh` to have a link show up in the navbar.

pgdp.net uses MediaWiki and the MediaWiki_extensions directory
includes some useful extensions you might want to use.

It's also suggested that you use the
[MediaWiki_PHPBB_Auth plugin](https://github.com/Digitalroot/MediaWiki_PHPBB_Auth) to have the
wiki authenticate against phpBB for a single source of user credentials.

### Define a site administrator
The code is based on users having particular roles. To manage these,
a site administrator is needed. Assuming you are administering the
site and have registered yourself as a new user, sign into mysql (or
use the phpMyAdmin interface) and add a usersetting entry for the
username you registered with setting='sitemanager' and value='yes';

From the mysql command line, this would be:
```
mysql> INSERT INTO usersettings
       SET username='your_username', setting='sitemanager', value='yes';
```

### Consult Site Admin Notes
The `site_admin_notes.txt` contains information for site admins such as
configuring rounds, defining queues, etc.

### Install the modified `dp.cron`
`dp.cron` contains entries for various processes necessary for site
statistics and project archiving, as well as managing the project release
queues, various user notifications, and the like.

Check that the values inserted by the DP configuration script are correct,
then install the crontab onto your system as an appropriate user.