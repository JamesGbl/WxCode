<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Charles Sanders, Ryan N. Freebern, Francesco Montorsi
// Creation date: 1/9/2005
// RCS-ID: $Id: dboutput.inc.php,v 1.25 2009/01/24 22:04:25 frm Exp $
// Notes: defines the functions for rendering query results
////////////////////////////////////////////////////////////////////////////////////////////


require_once("viewopt.inc.php");     // for VIEWOPT_ defines



// little helper function for query_table
function write_col($colname, $value, $tdstyle = "", $tdattr = "", $uselegend = FALSE)
{
    // do we have to add a style to this column ?
    if ($tdstyle) $tdstyle = " style='" . $tdstyle . "'";

    // write the column
    echo "<td $tdstyle $tdattr>";
    if ($uselegend)
        echo "<a class='compcol' href='legend.php#$colname'>$colname</a>: ";
    else
        echo "<span class='compcol'>$colname: </span>";
    echo "<span class='fieldValue'>$value</span></td>";
}

// used when viewmode == SINGLETABLE
function write_col_forsingletable($value, $link = "", $class = "fieldValue")
{
    echo "<td>";
    if ($link)
        echo "<span class='$class'><a href='$link'>$value</a></span>";
    else
        echo "<span class='$class'>$value</span>";
    echo "</td>";
}

// little helper function for query_table
function write_helper_link($link, $linklabel, $writeseparator = TRUE, $docheck = TRUE)
{
    $file_to_check = SERVERBASEURL . substr($link, strlen(SITEBASEURL));
    if ($docheck) {
        //echo "\n<!-- checking for '$file_to_check' -->\n";
        if (!file_exists($file_to_check))
            echo "&rsaquo; $linklabel";
        else
            echo "<a href='$link'>&rsaquo; $linklabel</a>";
    }
    else
        echo "<a href='$link'>&rsaquo; $linklabel</a>";
    if ($writeseparator) echo "&nbsp;&nbsp;|&nbsp;&nbsp;";
}

// little helper function for format_fieldvalues
function strTruncate($string, $limit, $pad="...")
{
    // return with no change if string is shorter than $limit
    if (strlen($string) <= $limit) return $string;

    // is $break present between $limit and the end of the string ?
    $breakpoint = strpos($string, '.', $limit);
    if ($breakpoint === FALSE or $breakpoint > strlen($string) - 1)
        $breakpoint = strpos($string, ',', $limit);
    if ($breakpoint === FALSE or $breakpoint > strlen($string) - 1)
        $breakpoint = $limit;     // cannot break in nice way... truncate it roughly

    assert($breakpoint >= $limit and $breakpoint <= strlen($string) - 1);
    return substr($string, 0, $breakpoint) . $pad;
}


// A collection of functions that render query results in XHTML
class DbOutput {

    // Outputs a single component
    function comp($compname)
    {
        // do a quick query for a specific component
        global $dbconn;
        db_connect();       // be sure to be connected

        $result = mysql_query("SELECT * FROM components WHERE name='$compname'", $dbconn);
        if ($result == FALSE || mysql_num_rows($result) != 1) {
            echo mysql_errno() . ": " . mysql_error() . "\n";
            die("<p>Could not find this component in the database! Please contact the project admin.</p>");
        }

        // get the maintainer row from the "maintainers" table
        $comp = mysql_fetch_assoc($result);
        $result2 = mysql_query("SELECT * FROM maintainers WHERE id='$comp[maintainerid]'", $dbconn);
        $numresult2 = mysql_num_rows($result2);

        if ($result2 == FALSE || ($numresult2 != 1 && $numresult2 != 0)) {
            echo mysql_errno() . ": " . mysql_error() . "\n";
            die("<p>Could not find the maintainer for this component in the database! Please contact the project admin.</p>");
        }

        // and output it without truncating the description
        if ($numresult2 == 1)
            $maintainer = mysql_fetch_assoc($result2);
        else
            $maintainer = DbQuery::db_get_unmaintained_maintainer();

        $view = new ViewOptAndPos();
        $view->opt->viewmode = VIEWOPT_VIEWMODE_FULL;
        DbOutput::queryres_table(0, $comp, $maintainer, $view, FALSE);
    }

    // Outputs the given database results
    function queryres($result, $view)
    {
        global $maintainers_table;

        // cache all the registered maintainers for faster rendering of components
        DbQuery::db_cache_maintainers_table();

        // begin the output
        DbOutput::queryres_table_begin($view);

        // Now, it is just a case of looping through the returned data and
        // output it in XHTML format
        $i = 0;
        while ($row = mysql_fetch_assoc($result)) {

            // do not break the table structure when we are inside a table
            if ($view->opt->viewmode != VIEWOPT_VIEWMODE_SINGLETABLE and $i > 0)
                echo "<br/>\n\n";
            if ($view->opt->viewmode != VIEWOPT_VIEWMODE_SINGLETABLE)
                write_h1("$row[name]");

            // find the maintainer for this component
            $rowmaintainer = NULL;
            foreach ($maintainers_table as $maintainer) {
                if ($maintainer['id'] == $row['maintainerid']) {
                    $rowmaintainer = $maintainer;
                    break;
                }
            }

            if ($rowmaintainer == NULL)
                die("<p>Could not find a maintainer for this component....</p>");

            // output this component
            DbOutput::queryres_table($i, $row, $maintainer, $view);
            $i++;
        }

        // end the output
        DbOutput::queryres_table_end($view);

        // Free the memory used for this result set
        mysql_free_result($result);
    }

    // Nicely formats the info of the given component
    function format_fieldvalues(& $comp)
    {
        // correct some values which are stored in the DB with lowercase names
        // but must be shown in mixed case
        switch ($comp['language']) {
            case "cpp": $comp['language'] = "C++"; break;
            case "wxlua": $comp['language'] = "wxLua"; break;
            case "wxpython": $comp['language'] = "wxPython"; break;
            case "wxperl": $comp['language'] = "wxPerl"; break;
        }

        // for multiple selections we have comma-separed strings which cannot
        // be handled with SWITCHes...
        $comp['buildsys'] = str_replace("bakefile", "Bakefile-based", $comp['buildsys']);
        $comp['buildsys'] = str_replace("cmake", "CMake-based", $comp['buildsys']);
        $comp['buildsys'] = str_replace(",", ", ", $comp['buildsys']);
        if ($comp['buildsys'] == "")
            $comp['buildsys'] = "not provided";

        $comp['wxport'] = str_replace("wxmsw", "wxMSW", $comp['wxport']);
        $comp['wxport'] = str_replace("wxgtk", "wxGTK", $comp['wxport']);
        $comp['wxport'] = str_replace("wxx11", "wxX11", $comp['wxport']);
        $comp['wxport'] = str_replace("wxmotif", "wxMotif", $comp['wxport']);
        $comp['wxport'] = str_replace("wxmac", "wxMac", $comp['wxport']);
        $comp['wxport'] = str_replace(",", ", ", $comp['wxport']);
        if ($comp['wxport'] == "")
            $comp['wxport'] = "none";

        $comp['wxversion'] = str_replace(",", ", ", $comp['wxversion']);
        $comp['status'] = strtoupper($comp['status']);

        // replace the most common symbols of description text which could make our markup invalid
        $comp['description'] = htmlentities($comp['description']);
        $comp['description'] = htmlspecialchars($comp['description']);
    }

    // Writes the header of the table for rendering the given viewmode
    function queryres_table_begin($view)
    {
        if ($view->opt->viewmode != VIEWOPT_VIEWMODE_SINGLETABLE)
            return;

        // in single table mode we output only the name,
        // the latest version, the status & the description
        echo "\n\n\n<table class='complistsingletable' cellspacing='0' cellpadding='10'>\n";
        echo "<tr class='complistsingletable_header'>\n";
        write_col_forsingletable("#", "", "");
        write_col_forsingletable("Name", "", "");
        write_col_forsingletable("Latest version", "", "");
        write_col_forsingletable("Status", "", "");
        write_col_forsingletable("Description", "", "");
        echo "</tr>\n\n";
    }

    // Writes the footer of the table for rendering the given viewmode
    function queryres_table_end($view)
    {
        if ($view->opt->viewmode != VIEWOPT_VIEWMODE_SINGLETABLE)
            return;

        echo "</table>\n\n";
    }

    // Writes the table for the component with given view options
    function queryres_table($i, $comp, $maintainer, $view, $truncatedesc = TRUE)
    {
        DbOutput::format_fieldvalues($comp);

        /* if the location field does not start with 'http://' then the
           component is located at wxCode */
        $hostedhere = (substr($comp['location'], 0, 7) != "http://");

        /* collect the URLs about this component in our COMPURL array */
        $compurl = array();
        $compurl['website'] = SITEBASEURL . "components/$comp[location]/";
        $compurl['showcomp'] = SITEBASEURL . "showcomp.php?name=$comp[name]";
        $compurl['wiki'] = WIKIBASEURL . "WxCode:$comp[name]"; // must not end with '/'
        $compurl['docs'] = SITEBASEURL . "docs/$comp[location]/";
        $compurl['screenshots'] = SITEBASEURL . "screenshots/$comp[location]/";
        $compurl['image'] = SITEBASEURL . "components/$comp[location]/image.png";
        $compurl['showthisonly'] = SITEBASEURL . "showcomp.php?name=$comp[name]";
        $compurl['cvs'] = "http://wxcode.cvs.sourceforge.net/wxcode/wxCode/components/$comp[location]";
        $compurl['svn'] = "http://wxcode.svn.sourceforge.net/viewvc/wxcode/trunk/wxCode/components/$comp[location]";
        $compurl['download'] = "https://sourceforge.net/projects/wxcode/files/Components";
        $compurl['mail'] = "mailto:" . str_replace("@", "@NOSPAM!@", $maintainer['mail']);

        /* for more info about this url see edit.php; it uses a POST/GET variable currcomp
           to determine which component is the maintainer editing... */
        $compurl['edit'] = SITEBASEURL . "edit.php?currcomp=$comp[name]";

        if ($comp['wiki'] == 'website')
            $compurl['website'] = $compurl['wiki'];
        if ($hostedhere == FALSE)
            $compurl['website'] = $comp['location'];

        // set the max lenght for the component description
        if ($view->opt->viewmode == VIEWOPT_VIEWMODE_FULL)
            $MAXDESC_LENGHT = 300;
        else
            $MAXDESC_LENGHT = 150;

        // truncate to MAXDESC_LENGHT chars the descriptions...
        if ($truncatedesc == TRUE && strlen($comp['description']) > MAXDESC_LENGHT)
            if (!file_exists(SERVERBASEURL . substr($compurl['website'], strlen(SITEBASEURL))))
                $comp['description'] = strTruncate($comp['description'], $MAXDESC_LENGHT);
            else    // pad in a nice way
                $comp['description'] = strTruncate($comp['description'], $MAXDESC_LENGHT,
                  "... [more info in the <a href='$compurl[website]'>component website</a>]");


        // now do the real output
        if ($view->opt->viewmode == VIEWOPT_VIEWMODE_FULL)
            DbOutput::queryres_table_foreachcomp($comp, $compurl, $maintainer, $view, $hostedhere);
        else
            DbOutput::queryres_table_single($comp, $compurl, $maintainer, $i);
    }

    // Writes the output table for a component when using the FULL viewmode
    function queryres_table_foreachcomp($comp, $compurl, $maintainer, $view, $hostedhere)
    {
        // begin to write the table specific for this component
        //   NB: the ID name for this table is very important since it is used by our
        //       JavaScript to dynamically show/hide the component tables...
        echo "<table id='comp_$comp[name]' class='complisttable'>";
        $rows_to_span = 6;

        if ($view->opt->viewmode == VIEWOPT_VIEWMODE_FULL) {

            /* if we do not force the width of the columns to a fixed % value, when
               the user moves the mouse over a link of this component (written at the
               end of this table), the columns change their sizes automatically
               making navigation much less pleasant ! */
            echo "<tr>";
            write_col("Categories", $comp['category'], "width:40%;");
            write_col("Status", $comp['status'], "width:40%;");
            if ($hostedhere == TRUE) {
                echo "<td rowspan='6' style='width:20%;'>";
                echo "<a href='$compurl[showthisonly]'>";

                // NB: the ALT attribute is required for valid XHTML !!
                echo "<img src='$compurl[image]' alt='Component logo' title='Click here to show this component only'/>";
                echo "</a>";
                echo "</td>";
            }
            echo "</tr>\n";

             /* write the two columns containing all the info about this component */
            echo "<tr>\n";
            write_col("Author", $comp['author']);
            write_col("wxWidgets supported ports", $comp['wxport']);
            echo "</tr><tr>\n";
            if ($compurl['mail'] == "mailto:UNMAINTAINED")
              write_col("Current maintainer", $maintainer['name']);
            else
              write_col("Current maintainer", "<a href='$compurl[mail]'>$maintainer[name]</a>");
            write_col("wxWidgets supported versions", $comp['wxversion']);
            echo "</tr><tr>\n";
            write_col("Latest version", $comp['version']);
            write_col("Build system", $comp['buildsys']);
            echo "</tr><tr>\n";
            write_col("Programming language", $comp['language']);
            write_col("Samples provided", $comp['samples']);
            echo "</tr><tr>\n";
            write_col("External dependencies", $comp['extdep']);
            write_col("Documentation", $comp['docs']);
            echo "</tr>\n";

            /* an empty row for spacing */
            echo "<tr><td><p/></td></tr>\n";
        }

        // next row is for the description (longer field)
        echo "<tr>\n";
        write_col("Description", $comp['description'], "text-align: justify;", "colspan='3'");
        echo "</tr>\n";

        if ($view->opt->viewmode == VIEWOPT_VIEWMODE_FULL) {

            // an empty row for spacing
            echo "<tr><td><p/></td></tr>\n";

            // finally, the links
            echo "<tr>\n";
            echo "<td colspan='3'><span class='compcol'>Links for $comp[name]:</span>";
            echo "<p class='complink'>";
            write_helper_link($compurl['website'], "Website");

            if ($hostedhere == TRUE) {

                // shows all these links only if the component is hosted here at wxCode
                write_helper_link($compurl['screenshots'], "Screenshots");
                write_helper_link($compurl['docs'], "Documentation");

                // do not make any existence check on these as they should always exist
                if ($comp['inCVS'] == 1)
                    write_helper_link($compurl['cvs'], "Browse CVS", TRUE, FALSE);
                else
                    write_helper_link($compurl['svn'], "Browse Subversion", TRUE, FALSE);
                write_helper_link($compurl['download'], "Download", TRUE, FALSE);
            }

            // this component has an enabled wiki ?
            // NB: believe it or not but PHP has a function which directly makes uppercase only the
            //     first character of a string !
            /*if (file_exists("/tmp/persistent/wxcode/pmwiki.d/wiki.d/Components." . ucfirst($comp['name'])))
                write_helper_link($compurl['wiki'], "Wiki", FALSE, FALSE);
            else
                echo "&rsaquo; Wiki";

                 NOTE: wiki has been disabled because was mostly unused
            */

            /*write_helper_link($compurl['showthisonly'], "Show only this box");
                     -- takes too much space - already linked with the component logo */
            echo "<br/>";
            write_helper_link($compurl['edit'], "Edit this component's info", FALSE, FALSE);
            echo " (for maintainers only)";
            echo "</p></td></tr>\n";

        } else if ($viewmode == "compact") {

            echo "<tr><td><span class='compcol'>Quick links: </span><span class='fieldValue'>\n";
            write_helper_link($compurl['showthisonly'], "Show detailed info about this component", FALSE);
            echo "</span></td></tr>\n";
        }

        // this table is finished
        echo "</table>\n";
    }

    // Writes the output table for a component when using the SINGLETABLE viewmode
    function queryres_table_single($comp, $compurl, $maintainer, $i)
    {
        if ($i % 2 == 0)
            echo "<tr class='complistsingletable_row1'>\n";
        else
            echo "<tr class='complistsingletable_row2'>\n";
        write_col_forsingletable($i + 1);
        write_col_forsingletable($comp['name'], $compurl['showcomp']);
        write_col_forsingletable($comp['version']);
        write_col_forsingletable($comp['status']);
        write_col_forsingletable($comp['description']);
        echo "</tr>\n";

        // an empty row for spacing
        //echo "<tr/>\n";
    }
}
