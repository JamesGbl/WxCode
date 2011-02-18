#!/usr/bin/php -q
<?php

    // Author: Francesco Montorsi
    // Purpose: connects the wxCode DB and updates the cached links to the components
    //          NOTE: this script is written in PHP so that we can use
    //          1) the better PHP support for MySQL connections
    //          2) the dbconnection.inc.php file which already contains the info required
    //             for DB connection (like user & password) so that we do not have to duplicate
    //             them here...

    $logsys_disabled = TRUE;        // required if we want to include globals.inc.php
    require("../dbconnection.inc.php");
    require("../viewopt.inc.php");
    require("../dbquery.inc.php");
    require("../globals.inc.php");

    define("OUTPUT_DIR", "/home/project-web/wxcode/htdocs/");


    function db_getnames()
    {
        // all components of the DB should begin with one of these chars (no case-sensitive):
        $alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        // break the string into an array
        $length = strlen($alphabet);
        $output = array();
        for($i = 0; $i < $length; $i++)
            $output[$i] = substr($alphabet, $i, 1);
        return $output;
    }

    function db_getcatname($cat, $browseby)
    {
        if ($browseby == VIEWOPT_BROWSEBY_NAME)
            return $cat;        // it should already be uppercase

        return strtoupper(substr($cat, 0, 1)) . substr($cat, 1);
    }

    function db_getcategories()
    {
        global $dbconn;

        $result = mysql_query("SHOW COLUMNS FROM components LIKE '%category%'", $dbconn);
        if ($result == FALSE) return -1;
        $row = mysql_fetch_row($result);

        // MySQL will output something like:
        // +----------+-------------------------------+-----+
        // | Field    | Type                          | ... |
        // +----------+-------------------------------+-----+
        // | category | set('first', 'second', ...)   | ... |
        // +----------+-------------------------------+-----+
        // so we need to parse the second column to get the available categories...
        $str = $row[1];
        if (strncmp($str, "set('", 5) != 0) {
            echo "Error: I expected $str to start with 'set(''...";
            exit;
        }

        // remove "set("
        $str = substr($str, 4);

        // remove last ")"
        $str = substr($str, 0, -1);

        $categories = convertStrToArray($str);
        $n = count($categories);

        // remove unwanted symbols
        for ($i = 0; $i < $n; $i++)
            $categories[$i] = str_replace("'", "", $categories[$i]);

        mysql_free_result($result);
        return $categories;
    }

    function write_sep($output)
    {
        if (!fwrite($output , " | \n")) {
            echo "Cannot write in the current output file !\n";
            exit;
        }
    }

    function update_links($output, $filename, $viewopt)
    {
        global $dbconn;
        if ($viewopt->browseby == VIEWOPT_BROWSEBY_CATEGORY)
            $cat = db_getcategories();
        else
            $cat = db_getnames();       // assume by name

        // while cycling, remember the number of components for each category
        $numcomps = array();

        // for all sections/categories...
        for ($i = 0; $i < count($cat); $i++) {
            echo "Looking how many components are under category '$cat[$i]'... ";

            // set current view position together with current view options
            $view = new ViewOptAndPos();
            $view->opt = $viewopt;
            $view->pos->section = $cat[$i];
            // $view->pos->page won't be used !

            // do we have any component in this category ?
            $q = "SELECT COUNT(name) FROM components WHERE " . DbQuery::get_mysql_where_clause($view);
            //echo "query is $q\n\n";
            $result = mysql_query($q, $dbconn);
            if ($result == FALSE) return -1;

            $row = mysql_fetch_row($result);
            $numrows = $row[0];
            mysql_free_result($result);

            echo "$numrows found\n";
            //array_push($numcomps, $numrows);
            $numcomps[$cat[$i]] = $numrows;

            // output links only to the sections which have a non-zero # of components
            if ($numrows > 0) {
                $catname = db_getcatname($cat[$i], $viewopt->browseby);
            echo "Writing $numrows component(s) links for category $catname...\n";

            // if previous section was written, then place a separator
            if ($i > 0) write_sep($output);

                if ($viewopt->compperpage == VIEWOPT_COMPPERPAGE_PARTIAL) {

                    // don't repeate the category name all the times; print it once
                    // before the page links
                    fwrite($output, "$catname&nbsp;($numrows):&nbsp;");
                    $numpages = ceil($numrows / $viewopt->compperpage);

                    for ($j = 0; $j < $numpages; $j++) {
                        $view->write_cached_page_link_tofile($output, $filename, $j + 1, $j + 1);
                        if ($j < $numpages) fwrite($output, "&nbsp;");
                    }

                } else {

                    // a single page link for all components of this section...
                    $view->write_cached_page_link_tofile($output, $filename, "$catname&nbsp;($numrows)", 1);
                }
            }
        }

        // save in this cached file also the number of components in each section (as PHP array)
        fwrite($output, "\n\n<" . "?php global \$numcomps_serialized;\n" .
                        "\$numcomps_serialized = '" . addcslashes(serialize($numcomps), "\'") . "'\n ?>");
    }

    function update_all_links() {
        $filename = "complist.php";

        // we have to regenerate the page links for the following options:
        //
        // VIEWMODE: "full" or "singletable"
        // BROWSEBY: "name" or "category"
        // COMPPERPAGE: COMPPERPAGE_PARTIAL or COMPPERPAGE_ALL
        //
        // thus we have to regen 2 x 2 x 2 = 8 XHTML pages.

        $viewmode_arr = array(VIEWOPT_VIEWMODE_FULL, VIEWOPT_VIEWMODE_SINGLETABLE);
        $browseby_arr = array(VIEWOPT_BROWSEBY_NAME, VIEWOPT_BROWSEBY_CATEGORY);
        $compperpage_arr = array(VIEWOPT_COMPPERPAGE_PARTIAL, VIEWOPT_COMPPERPAGE_ALL);

        for ($i = 0; $i < count($viewmode_arr); $i++) {
            for ($j = 0; $j < count($browseby_arr); $j++) {
                for ($k = 0; $k < count($compperpage_arr); $k++) {

                    // build our view options
                    $viewopt = new ViewOpt();
                    $viewopt->viewmode = $viewmode_arr[$i];
                    $viewopt->browseby = $browseby_arr[$j];
                    $viewopt->compperpage = $compperpage_arr[$k];

                    // create the output filename with a precise rule
                    $outputfn = OUTPUT_DIR . $viewopt->get_cachedlinks_pagename($viewopt);

                    // open the output file
                    echo "Writing $outputfn...\n";
                    if (!is_writeable($outputfn))
                        echo "The file does not exist... trying to create it.\n";
                    if (!$output = fopen($outputfn, "w")) {
                        echo "Cannot write into $outputfn !\n";
                        exit;
                    }

                    // now write it
                    update_links($output, $filename, $viewopt);
                    fclose($output);
                }
            }
        }
    }


    // HERE STARTS THE "main" OF THIS SCRIPT

    // call the db_connect function so it define the constants we need
    db_connect_fromscript();

    // just update all our page links
    update_all_links();
?>