<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 28/8/2005
// RCS-ID: $Id: viewopt.inc.php,v 1.11 2008/11/06 15:33:14 frm Exp $
// Notes: defines the functions & constants used to show lists of components
//        in the COMPLIST.PHP and in the SEARCHSHOW.PHP files.
////////////////////////////////////////////////////////////////////////////////////////////

// NOTES ABOUT COMPONENT LISTS RENDERING:
//
// The view options are:
// 1) viewmode
// 2) components per page
// 3) browse by
//
// NOTE: all functions will get these info by a ViewOpt class
//
// wxCode website uses two different systems to show the components:
// 1) COMPLIST.PHP uses the 'categorized' approach: i.e. when browsing by
//    name/category a nice list of cached links are shown to the user that
//    in this way can see how many components are in which categories
//
// 2) SEARCHSHOW.PHP uses the simplest approach with plain links all named
//    "Page 1", "Page 2", etc. This is because it's too slow to generate all
//    the page links in a form like COMPLIST.PHP uses in real-time.
//
// Once the view options has been chosen, to be able to render a list of components
// also the following info are required:
// 1) the current SECTION: maybe 'A', 'B', 'T', 'Z' or 'Miscellaneous', 'Windows', etc
// 2) the current PAGE inside the current section: must be >= 1
//
// NOTE: all functions will get these info by a ViewPos class



// defines
// --------------

define("VIEWOPT_VIEWMODE_FULL", "full");
define("VIEWOPT_VIEWMODE_SINGLETABLE", "singletable");


define("VIEWOPT_COMPPERPAGE_PARTIAL", "10");

// we need a positive number when doing view calculations in VIEWOPT_COMPPERPAGE_ALL mode,
// so, instead of using a value like "-1" which would mean 'show all components regardless of how many they are',
// we use a BIG value like "1000" so that we can *avoid* to do such kind of checks:
//
//  $numcomp = $view->compperpage;
//  if ($view->compperpage == VIEWOPT_COMPPERPAGE_ALL)
//     $numcomp = 1000;
//
// all the times we need to use the ViewOpt::compperpage variable
// (it would be necessary if this constant was defined to a value like "-1") !
define("VIEWOPT_COMPPERPAGE_ALL", "1000");


define("VIEWOPT_BROWSEBY_NAME", "name");
define("VIEWOPT_BROWSEBY_CATEGORY", "category");

define("VIEWOPT_CACHEFILES_SUFFIX", ".inc.html");



// classes
// --------------

class ViewOpt {
    var $viewmode, $compperpage, $browseby;


    // Returns the name of the file containing the cached page links
    // for the current view options.
    function get_cachedlinks_pagename()
    {
        $tmp = array(VIEWOPT_COMPPERPAGE_PARTIAL => "10",
                     VIEWOPT_COMPPERPAGE_ALL => "all");

        // create a file name unique for this set of view options
        return "cachedlinks_" . $this->viewmode . "_" . $this->browseby . "_" .
               $tmp[$this->compperpage] . VIEWOPT_CACHEFILES_SUFFIX;
    }

    // Gets view options from POST vars, if present
    function get_from_post()
    {
        if (isset($_REQUEST['compperpage']))
          $this->compperpage=$_REQUEST['compperpage'];
        else if (isset($_SESSION['compperpage']))
          $this->compperpage=$_SESSION['compperpage'];
        else                                          // set the default value
          $this->compperpage=VIEWOPT_COMPPERPAGE_ALL;

        if (isset($_REQUEST['browseby']))
          $this->browseby=$_REQUEST['browseby'];
        else if (isset($_SESSION['browseby']))
          $this->browseby=$_SESSION['browseby'];
        else                                          // set the default value
          $this->browseby=VIEWOPT_BROWSEBY_CATEGORY;

        if (isset($_REQUEST['viewmode']))
          $this->viewmode=$_REQUEST['viewmode'];
        else if (isset($_SESSION['viewmode']))
          $this->viewmode=$_SESSION['viewmode'];
        else                                          // set the default value
          $this->viewmode=VIEWOPT_VIEWMODE_FULL;

        assert($this->compperpage != "");
        assert($this->viewmode != "");
        assert($this->browseby != "");

        /* FOR DEBUGGING ONLY !*/
        echo "\n<!--\n";
        var_dump($this->viewmode);
        var_dump($this->browseby);
        var_dump($this->compperpage);
        echo "-->\n\n";
        /**/
    }
}

class ViewPos {
    var $section, $page;


    // Gets view current pos from POST vars, if present
    function get_from_post($opt)        // opt must be of kind ViewOpt
    {
        assert(get_class($opt) == "ViewOpt");

        if (isset($_REQUEST['page']))
          $this->page=$_REQUEST['page'];
        else if (isset($_SESSION['page']))
          $this->page=$_SESSION['page'];
        else                                          // set the default value
          $this->page=1;      // page must always be >= 1

        if (isset($_REQUEST['section']))
          $this->section=$_REQUEST['section'];
        else if (isset($_SESSION['section']))
          $this->section=$_SESSION['section'];
        else                                          // set the default value
          if ($opt->browseby == VIEWOPT_BROWSEBY_NAME)
              $this->section="A";
          else
              $this->section="Miscellaneous";

        assert($this->section != "");
        assert($this->page != "");

        /* FOR DEBUGGING ONLY !*/
        echo "\n<!--\n";
        var_dump($this->section);
        var_dump($this->page);
        echo "-->\n\n";
        /**/
    }
}

class ViewOptAndPos {
    var $opt, $pos;

    function ViewOptAndPos() {
        $this->opt = new ViewOpt();
        $this->pos = new ViewPos();
    }

    // Gets view options & current pos from POST vars, if present
    function get_from_post()
    {
        $this->opt->get_from_post();
        $this->pos->get_from_post($this->opt);
    }

    // Save all current data in SESSION vars FIXME: broken
    function save_in_session()
    {
        // save the view options in SESSION variables...
        $_SESSION['page'] = $page;
        $_SESSION['viewmode'] = $viewmode;
        $_SESSION['compperpage'] = $compperpage;
        $_SESSION['browseby'] = $browseby;
        $_SESSION['section'] = $section;
    }

    function get_page_link_href_tofile($filename, $page, $cached = FALSE)
    {
        // encode in the url the view options
        $ret = "$filename?compperpage={$this->opt->compperpage}";
        $ret .= "&amp;viewmode={$this->opt->viewmode}";
        $ret .= "&amp;browseby={$this->opt->browseby}";

        // then view position
        if ($cached) {
            $sect = urlencode($this->pos->section);     // section could contain spaces
            $ret .= "&amp;section={$sect}";      // SECTION IS NOT USED IN PLAIN PAGE LINKS
        }
        $ret .= "&amp;page={$page}";

        return $ret;
    }

    // Writes an anchor to the given filename with the given label
    // You must overload the current page number to link.
    // This method is used when creating plain page links (instead of cached ones).
    function write_page_link_tofile($filename, $label, $page, $cached = FALSE)
    {
        echo "<a href='" . ViewOptAndPos::get_page_link_href_tofile($filename, $page, $cached) .
             "'>$label</a>";
    }

    // Writes an anchor to the given filename with the given label
    // You must overload the current page number to link.
    // This method is used when creating plain page links (instead of cached ones).
    function write_cached_page_link_tofile($output, $filename, $label, $page)
    {
        fwrite($output, "<a href='" . ViewOptAndPos::get_page_link_href_tofile($filename, $page, TRUE) .
                        "'>$label</a>");
    }

    // Writes the links for all the pages
    // This method is used when creating plain page links (instead of cached ones).
    function write_all_page_links($filename, $numcomponents, $usepagelinkdiv = FALSE, $cached = FALSE)
    {
        if ($this->opt->viewmode == VIEWOPT_VIEWMODE_SINGLETABLE)
            return;     // these links are not required when we are in singletable mode
        $filename .= ".php";

        $currpage = $this->pos->page;
        assert($currpage >= 1);
        $numpages = ceil($numcomponents / $this->opt->compperpage);

        if ($usepagelinkdiv)
            echo "<div class='pagelink'><p>";

        // the PREV PAGE link
        if ($currpage > 1) {
            ViewOptAndPos::write_page_link_tofile($filename,
                "&lsaquo;&lsaquo; Previous", $currpage - 1, $cached);
            echo " | ";
        }

        // here we need the PAGE links
        for ($i=1; $i <= $numpages; $i++) {
            if ($i == $currpage)
                echo "Page $i"; // this is the page which we are currently showing...
            else
                ViewOptAndPos::write_page_link_tofile($filename, "Page $i", $i, $cached);

            if ($i < $numpages)
                echo " | ";
        }

        // the NEXT PAGE link
        if ($currpage < $numpages) {
            echo " | ";
            ViewOptAndPos::write_page_link_tofile($filename,
                "Next &rsaquo;&rsaquo;", $currpage + 1, $cached);
        }

        if ($usepagelinkdiv)
            echo "</p></div>";
    }

    // Computes a lot of variables useful to render component lists in plain mode.
    function compute_view_stuff($numrows, & $offset, & $numpages)
    {
        global $dbconn;
        db_connect();     // be sure to be connected

        // we need to know how many lines we are working with
        assert(isset($numrows) and !empty($numrows) and $numrows >= 1);

        // check how many pages in total are needed, and if $page is greater
        // than that, set $page to the last page
        $numpages = ceil($numrows / $this->opt->compperpage);
        if ($this->pos->page > $numpages)
            $this->pos->page = $numpages;

        // now we know what page we're on, how many rows we're working with,
        // and how many rows to display per page: we can work out the offset
        // of the first record to show
        $offset = $this->opt->compperpage * ($this->pos->page - 1);
    }

    // Includes the cached page links for the current view mode.
    function write_cached_page_links($usepagelinkdiv = FALSE)
    {
        if ($usepagelinkdiv)
            echo "<div class=\"pagelink\"><p>";

        // instead of making a lot of database queries, we use a precalculated link page
        $fn = $this->opt->get_cachedlinks_pagename();
        echo "\n\n<!-- including file $fn -->\n";
        require($fn);

        assert(isset($numcomps_serialized));
        global $numcomps;
        $numcomps = unserialize($numcomps_serialized);

        if ($usepagelinkdiv)
            echo "</p></div>\n\n";
    }

    function write_page_links_for_section($filename, $numcomparr)
    {
        assert(is_array($numcomparr));

        // write the page links for this section only
        $n = $numcomparr[$this->pos->section];
        $this->write_all_page_links($filename, $n, FALSE, TRUE);
        echo "<br/>";        // to keep !
    }

    // Write the name of the current "view position"
    function write_current_pos()
    {
        echo "&gt;&gt; Current section: <em>{$this->pos->section}</em> - ";
        echo "Current page: <em>{$this->pos->page}</em> &lt;&lt;<br/>";
    }
}
?>
