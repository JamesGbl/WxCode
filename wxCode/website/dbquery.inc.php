<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Charles Sanders, Ryan N. Freebern, Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: dbquery.inc.php,v 1.69 2009/10/16 11:58:41 frm Exp $
// Notes: defines some functions for easier querying the database
////////////////////////////////////////////////////////////////////////////////////////////


require_once("viewopt.inc.php");     // for VIEWOPT_ defines
require_once("comp.inc.php");     // for Component & Maintainer classes


// A collection of query helper functions (contains no data); call these functions writing:
//
//    $res = DbQuery::funcname(...);
//
class DbQuery {

    // Returns the number of registered components in the database
    function numcomp($only_approved = TRUE)
    {
        global $dbconn;
        db_connect();     // be sure to be connected

        // build query
        $query = "SELECT COUNT(name) FROM components";
        if ($only_approved) $query .= " WHERE approved='1'";
        $result = mysql_query($query, $dbconn);
        if ($result == FALSE) return -1;

        $row = mysql_fetch_row($result);
        $numrows = $row[0];
        mysql_free_result($result);

        return $numrows;
    }

    // Returns the number of unmaintained components in the database
    function numcomp_unmaint()
    {
        global $dbconn;
        db_connect();     // be sure to be connected

        // build query
        $query = "SELECT COUNT(name) FROM components WHERE maintainerid='0'";
        $result = mysql_query($query, $dbconn);
        if ($result == FALSE) return -1;

        $row = mysql_fetch_row($result);
        $numrows = $row[0];
        mysql_free_result($result);

        return $numrows;
    }

    // Returns the number of components maintained by the given username
    function numcomp_for_maintainer($id)
    {
        global $dbconn;
        db_connect();   // be sure to be connected

        $result = mysql_query("SELECT name FROM components WHERE maintainerid='$id'", $dbconn);
        $numcomps = mysql_num_rows($result);
        mysql_free_result($result);

        return $numcomps;
    }

    // Returns the number of maintainers registered in the wxCode database
    function nummaint()
    {
        global $dbconn;
        db_connect();   // be sure to be connected

        $query = "SELECT COUNT(name) FROM maintainers";
        $result = mysql_query($query, $dbconn);
        if ($result == FALSE) return -1;

        $row = mysql_fetch_row($result);
        $numrows = $row[0];
        mysql_free_result($result);

        return $numrows;
    }

    // Returns the maintainer ID for the given maintainer SF user name.
    function maintainer_id($sfuser)
    {
        global $dbconn;
        db_connect();     // be sure to be connected

        $result = mysql_query("SELECT id FROM maintainers WHERE sfuser='$sfuser'", $dbconn);
        if ($result == FALSE) return -1;

        $row = mysql_fetch_row($result);
        $id = $row[0];
        mysql_free_result($result);

        return $id;
    }

    function get_mysql_where_clause($view)
    {
        assert(get_class($view) == "ViewOptAndPos");
        $sectname = $view->pos->section;

         if ($view->opt->browseby == VIEWOPT_BROWSEBY_CATEGORY) {

              // category is a SET and thus % are required
              $query = "( category LIKE '%$sectname%' )";

         } else {       // by name

              // A... and wxA... are the same for us
              if ($sectname == "W" || $sectname == "w")
                $query = "( name LIKE 'w%' AND name NOT LIKE 'wx%' )";
              else
                $query = "( name LIKE '$sectname%' OR name LIKE 'wx$sectname%' )";
      }

         return $query;
    }

    // Returns the components to show in the component list for the given ViewOptAndPos class
    function complist($view, $only_approved = TRUE, $ignoresection = FALSE)
    {
        assert(get_class($view) == "ViewOptAndPos");

        global $dbconn;
        db_connect();     // be sure to be connected

        // get some result-view-related stuff
        $compperpage = $view->opt->compperpage;
        $offset = ($view->pos->page - 1) * $compperpage;

        // we can construct the query, and pass in the limit clause
        $query = "SELECT * FROM components WHERE ";
        if (!$ignoresection)
            $query .= DbQuery::get_mysql_where_clause($view);      // this is the standard way

        // be careful to create an SQL query with the right syntax
        if ($ignoresection)
            $query .= "approved='1'";
        else if ($only_approved)
            $query .= " AND approved='1'";

        $query .= " ORDER BY name";
        $query .= " LIMIT $offset,$compperpage";

        // do the query
        $result = mysql_query($query, $dbconn);
        if ($result == FALSE || mysql_num_rows($result) < 1) {

            // We know we _should_ have a result, but we haven't
            echo "Offset was: $offset<br/>";
            echo "Query was: $query<br/>";
            echo mysql_errno() . ": " . mysql_error() . "\n";
            die("<p>No data retrieved!  Please contact the project admin.</p>");
        }

        return $result;
    }

    // Returns the components to show in the component list for the given ViewOptAndPos class
    function comp_to_approve()
    {
        global $dbconn;
        db_connect();     // be sure to be connected

        // we can construct the query, and pass in the limit clause
        $query = "SELECT * FROM components WHERE approved='0' ORDER BY name";

        // do the query
        $result = mysql_query($query, $dbconn);
        if ($result == FALSE) {     // NOTE: having zero results is ok here
            echo mysql_errno() . ": " . mysql_error() . "\n";
            die("<p>No data retrieved!  Please contact the project admin.</p>");
        }

        return $result;
    }

    // Returns the components to show for the given SEARCH request
    function searchlist($vararr, & $query, & $numrows, $view, & $numpages, $only_approved = TRUE)
    {
        global $dbconn;
        db_connect();     // be sure to be connected

        // initially, do not set the LIMIT or ORDER keywords
        $category = convertArrayToStr($vararr['category']);
        $buildsys = convertArrayToStr($vararr['buildsys']);
        $wxversion = convertArrayToStr($vararr['wxversion']);
        $wxport = convertArrayToStr($vararr['wxport']);

        $category = str_replace(",", "%", $category); $category = "%" . $category . "%";
        $buildsys = str_replace(",", "%", $buildsys); $buildsys = "%" . $buildsys . "%";
        $wxversion = str_replace(",", "%", $wxversion); $wxversion = "%" . $wxversion . "%";
        $wxport = str_replace(",", "%", $wxport); $wxport = "%" . $wxport . "%";

        $whereclause = "WHERE name LIKE '$vararr[name]' AND author LIKE '$vararr[author]' " .
                       "AND extdep LIKE '$vararr[extdep]' AND category LIKE '$category' " .
                       "AND language LIKE '$vararr[language]' AND description LIKE '$vararr[description]' ";

        if ($vararr['status'] == "UNMAINTAINED")
            $whereclause .= "AND maintainerid='0' ";
        else
            $whereclause .= "AND status LIKE '$vararr[status]' ";

        $whereclause .= "AND docs LIKE '$vararr[docs]' " .
                        "AND (wxport LIKE '$wxport' OR wxport LIKE '%all%') " .
                        "AND (wxversion LIKE '$wxversion' OR wxversion LIKE '%all%') " .
                        "AND buildsys LIKE '$buildsys'";

        if ($only_approved)
            $whereclause .= " AND approved='1'";
        else
            $whereclause .= "";

        // to be able to compute the number of pages to show, we need to get the number of results...
        $query = "SELECT name FROM components $whereclause";
        $result = mysql_query($query, $dbconn);
        if ($result == FALSE || mysql_num_rows($result) < 1)
            die("<p class='error'>Sorry; no results found for the query: '$query'...</p>");

        $offset = 0;
        $numrows = mysql_num_rows($result);

        // comput the offset and numpages
        $view->compute_view_stuff($numrows, $offset, $numpages);

        // finally, we can construct the query, and pass in the limit clause
        $num = $view->opt->compperpage;
        $query = "SELECT * FROM components ";
        $query .= $whereclause;
        $query .= " ORDER BY {$view->opt->browseby}";

        // LIMIT syntax is:
        //                  LIMIT a,b
        //  where
        //     a = the position of the first record to query
        //     b = the number of records to query
        $query .= " LIMIT $offset,$num";

        // do query & check results
        $result = mysql_query($query, $dbconn);
        if ($result == FALSE || mysql_num_rows($result) < 1) {

            // We know we _should_ have a result, but we haven't
            echo "<p class='error'>Could not perform the query !<br/>";
            echo "Query was: $query<br/>";
            echo "MySQL error number " . mysql_errno() . ": " . mysql_error() . "<br/>";
            die("No data retrieved! Please contact the project admin.</p>");
        }

        return $result;
    }

    // Returns an array which contains the info about the UNMAINTAINED "maintainer"
    // organized as they are in the "maintainers" table
    function db_get_unmaintained_maintainer()
    {
        // create a new array with fields like those of the "maintainers" database table
        return array("id" => "0",
                     "mail" => "UNMAINTAINED",
                     "sfuser" => "",
                     "name" => "UNMAINTAINED<br/>&nbsp;&nbsp;(<a href='help.php'>Want&nbsp;to&nbsp;be&nbsp;the maintainer&nbsp;of&nbsp;this one&nbsp;?</a>)",
                     "password" => "",
                     "approved" => "1");
    }

    // Caches the maintainer table in a global var which is then returned.
    function db_cache_maintainers_table()
    {
        global $maintainers_table, $dbconn;

        $result = mysql_query("SELECT * FROM maintainers", $dbconn);
        if ($result == FALSE || mysql_num_rows($result) < 1) {
            echo mysql_errno() . ": " . mysql_error() . "\n";
            die("<p>No data retrieved from maintainers table!  Please contact the project admin.</p>");
        }

        // init the maintainers table with a special entry: the UNMAINTAINED "maintainer"
        $unmaintained = DbQuery::db_get_unmaintained_maintainer();

        // push each result row in the global cache array $maintainers_table
        $maintainers_table = array($unmaintained);
        while ($row = mysql_fetch_assoc($result))
            array_push($maintainers_table, $row);

        return $maintainers_table;
    }
}
?>
