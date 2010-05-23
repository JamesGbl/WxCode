<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: header.inc.php,v 1.73 2008/11/06 15:41:49 frm Exp $
// Notes: this file is included just after the <html> tag by all the
//        files of the wxCode website. This file dictates the style of
//        all the website through the CSS which is here included.
//        Both this page and the CSS have been taken from 'Sinorca'
//        open source web design available at http://www.oswd.org
////////////////////////////////////////////////////////////////////////////////////////////
?>

<head>
  <meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8" />
  <meta name="author" content="Francesco Montorsi" />
  <meta name="keywords" content="wxCode, wxWidgets, component, modular, C++, download, compile, bakefile, maintainer" />

  <?php

      // define some useful constants
      if (!isset($filelocation))
          $filelocation = "";

      $usescript = FALSE;
      $logohack = FALSE;

      /* this makes debugging a lot easier */
      echo "<!-- $_SERVER[HTTP_USER_AGENT] -->";

      /* for broken browsers like IE we have to set as default the 'wxcode-notfixed.css' CSS
         NOTE: we won't use 'Mozilla' string to understand if this browser has or hasn't multiple
               CSS support: too many browsers use that... */
      if (isset($_SERVER["HTTP_USER_AGENT"]) and strstr($_SERVER["HTTP_USER_AGENT"], "Firefox"))
          $usescript = TRUE;

      /* for The Broken Browser (MS IE) we have to use a special hack also for displaying the logo */
      if (isset($_SERVER["HTTP_USER_AGENT"]) and strstr($_SERVER["HTTP_USER_AGENT"], "MSIE")
          and !strstr($_SERVER["HTTP_USER_AGENT"], "Mac_PowerPC") /* Mac IE has MSIE string of course but doesn't need the hack... */
          and !strstr($_SERVER["HTTP_USER_AGENT"], "Opera"))      /* Opera has MSIE string inside but doesn't need this hack! */
          $logohack = TRUE;
  ?>

  <?php if ($usescript) { ?>
    <link rel="stylesheet" type="text/css"
        href="<?php echo SITEBASEURL; ?>wxcode-contentscroll.css" media="screen" title="Frame-like" />
  <?php } ?>

  <link rel="stylesheet" type="text/css"
        href="<?php echo SITEBASEURL; ?>wxcode-allscroll.css" media="screen" title="Default" />

  <link rel="alternate" type="application/rss+xml" title="wxCode Project News"
        href="http://sourceforge.net/export/rss2_projnews.php?group_id=51305&amp;rss_fulltext=1" />
  <link rel="alternate" type="application/rss+xml" title="wxCode Forum News"
        href="http://wxforum.shadonet.com/rss.php?forums%5B%5D=30&amp;threadCount=50" />

  <!-- These three lines are required for Slimbox2. -->
  <link rel="stylesheet" href="<?php echo SITEBASEURL; ?>slimbox2.css" type="text/css" media="screen" />
  <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js"></script>
  <script type="text/javascript" src="<?php echo SITEBASEURL; ?>slimbox2.js"></script>

  <title>wxCode - <?php echo "$pagetitle"; ?></title>
</head>

<body >

<!-- ##### Header ##### -->
<div id="header" class="topframe">
  <div class="superHeader">
    <div class="left">

      <!-- RSS link -->
      <a href="http://sourceforge.net/export/rss2_projnews.php?group_id=51305&amp;rss_fulltext=1">RSS</a>
      <?php
        // we use the $logged_in var declared by login.inc.php
        if ($logged_in) {
          echo " | Welcome <em>$_SESSION[username]</em>; ";
          echo "you are logged in as wxCode maintainer (<a href='logout.php'>log out</a>)";
        } else if ($logsys_disabled) {
          // just don't say anything since in this page the log system is disabled
        } else {
          echo " | Not logged in (<a href='login.php'>log in</a>)";
        }
      ?>
    </div>

    <div class="right">
      <?php if ($usescript) { ?>
      Select the CSS style you prefer:

      <!-- here is important to use for the ID the same string contained
            as value in <a>...</a>  tags -->
      <a id="Frame-like" href="#" onclick="return onCSSselector('Frame-like', this);">Frame-like</a> |
      <a id="Default" href="#" onclick="return onCSSselector('Default', this);">Default</a>

      <!-- this allows the user to do a nice CSS choice

           VERY IMPORTANT: some browsers, like Firefox 3 beta, for some reason do not
                           accept the syntax <script ... />, so it's very important to
                           always write <script ...></script> instead!
      -->
      <script type="text/javascript" src="<?php echo SITEBASEURL; ?>styleswitcher.js">
      </script>
      <?php } ?>
    </div>
  </div>


  <!-- ====================================================================== -->
  <!-- =================== HERE IS THE WXCODE LOGO ========================== -->
  <!-- ====================================================================== -->
  <div class="midHeader">

    <a href="http://wxcode.sourceforge.net">
    <?php if ($logohack) { ?>

        <!-- another fix for that broken and damned IE !!!!!! it does not even support transparent PNG ! grrrrrrrrr -->
        <img alt="The wxCode logo" src="<?php echo SITEBASEURL; ?>images/blank.gif" style="width: 186px; height: 60px; filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(src='<?php echo SITEBASEURL; ?>images/wxcodelogo2.png', sizingMethod='scale')" />

    <?php } else { ?>

        <!-- The height parameter is very important as it makes the logo fit the header with any font size
             chosen by the user (i.e. it makes the logo resizeable) -->
        <img src="<?php echo SITEBASEURL; ?>images/wxcodelogo2.png" title="The wxCode logo" alt="The wxCode logo" style="height:5em;"/>

    <?php } ?>
    </a>

    <h2 class="headerSubtitle">
        <em>your <a href="http://www.wxwidgets.org">wxWidgets</a> links: </em>
        <a href="http://wiki.wxwidgets.org/">wxWiki</a> |
        <a href="http://wxforum.shadonet.com/viewforum.php?f=30">wxCode forum</a> |
        <a href="http://groups.google.com/group/comp.soft-sys.wxwindows">Mailing list</a>
    </h2>
  </div>

  <div class="subHeader">
    <table cellspacing="0" cellpadding="0"><tr>
      <td>
        <a href="http://wxcode.sourceforge.net">wxCode</a> &raquo;
        <?php
            if (substr($menuentry, 0, 2) == "<a")
                echo "$menuentry";      // this var already contains the Anchor tag
            else
                echo "<a href=\"" . FILEBASEURL . "$filename.php\">$menuentry</a>";
        ?>
    </td>
    <td style="text-align: right;">
        <object>       <!-- this is required for valid XML -->
            <form method="get" action="http://www.google.com/search">
                <div>       <!-- this is required for valid XML -->
                    <input type="text" name="q" maxlength="64" value="" class="searchTextCtrl"/>
                    &nbsp;&nbsp;&nbsp;
                    <input type="submit" name="btnG" value="Search this site" class="searchBtn"/>
                    <input type="hidden" name="domains" value="http://wxcode.sourceforge.net" class="searchBtn"/>
                    <input type="hidden" name="sitesearch" value="http://wxcode.sourceforge.net" class="searchBtn"/>
                </div>
            </form>
        </object>
    </td>
    </tr></table>
  </div>
</div>


<?php

  // find first occurance of needle in str, stripos is only in php 5,
  if (!function_exists("stripos"))
  {
    function stripos($str,$needle)
    {
      return strpos(strtolower($str),strtolower($needle));
    }
  }

  function writeSideBarEntry($entryname, $entrytitle)
  {
    global $filename, $filelocation;

    // write different HTML code if this the menu entry for this page
    $isthispage = ($filename == $entryname and $filelocation == "");
    if ($isthispage)
        echo "<span class='thisPage'>&nbsp;&nbsp;&nbsp;";
    else
        // here we use SITEBASEURL because the menu items are in the SITEBASE
        // unless they start with "http://", note === identical operator
        // since 0 == false
        // NOTE 7/5/06: I've removed the 'title' attribute here as it creates
        //              a tooltip which gives problems to some user agents and
        //              that in any case does not add any info to the link title
        if (stripos($entryname, "http://") === false)
            echo "<a href='" . SITEBASEURL . "$entryname.php'>";
        else
            echo "<a href='$entryname'>";

    echo "&rsaquo; $entrytitle";

    if ($isthispage)
        echo '</span>';
    else
        echo '</a>';
  }
?>

<!-- ##### Side Bar ##### -->
<div class="side-bar">
  <div>
    <p class="sideBarTitle">Users</p>
    <ul>
      <li><?php writeSideBarEntry("index", "Main page") ?></li>
      <li><?php writeSideBarEntry("complist", "Components") ?></li>
      <li><?php writeSideBarEntry("search", "Component<br/>&nbsp;&nbsp;&nbsp;search") ?></li>
      <li><?php writeSideBarEntry("faq", "FAQ") ?></li>
      <li><?php writeSideBarEntry("support", "Support") ?></li>
      <!--<li><?php writeSideBarEntry("wiki/pmwiki", "wxCode wiki") ?></li>-->
      <li><?php writeSideBarEntry("links", "Links and<br/>&nbsp;&nbsp;&nbsp;site map") ?></li>
      <li><?php writeSideBarEntry("http://sourceforge.net/projects/wxcode", "Sourceforge<br/>&nbsp;&nbsp;&nbsp;project page") ?></li>
    </ul>

    <p class="sideBarTitle">Maintainers</p>
    <ul>
      <li><?php writeSideBarEntry("submit", "Submission guide") ?></li>
      <li><?php writeSideBarEntry("rules", "Maintainer rules") ?></li>
      <li><?php writeSideBarEntry("maintguide", "Maintainer guide") ?></li>

      <li><?php writeSideBarEntry("template", "Project template") ?></li>
      <li><?php writeSideBarEntry("bakefile", "Bakefile docs") ?></li>
      <li><?php writeSideBarEntry("autoconf", "Autoconf docs") ?></li>

      <li><?php writeSideBarEntry("edit", "Edit your<br/>&nbsp;&nbsp;&nbsp;component") ?></li>
    </ul>

  </div>
</div>


<!-- ##### Main Content ##### -->
<div class="main-content">
  <div id="top"/>

