<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 28/8/2005
  // RCS-ID: $Id: cachedlinks.inc.php,v 1.2 2005/09/01 19:59:46 frm Exp $
  // Notes: defines the functions
  //        - get_cachedlinks_pagename
  ////////////////////////////////////////////////////////////////////////////////////////////<!--


define("OUTPUT_DIR", "/home/groups/w/wx/wxcode/htdocs");
define("OUTPUT_SUFFIX", ".inc.html");

define("COMPPERPAGE_PARTIAL", 10);
define("COMPPERPAGE_ALL", -1);

function get_cachedlinks_pagename($browseby, $compperpage) {
    $compperpagemode = array(COMPPERPAGE_PARTIAL => "partial", 
                            COMPPERPAGE_ALL => "all");
    return "complinks_" . $browseby . "_" . $compperpagemode[$compperpage] . OUTPUT_SUFFIX;
}

?>
