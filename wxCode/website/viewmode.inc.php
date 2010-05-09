<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 8/7/2005
// RCS-ID: $Id: viewmode.inc.php,v 1.9 2008/11/06 15:33:14 frm Exp $
// Notes: display the view options; see viewopt.inc.php for more info
////////////////////////////////////////////////////////////////////////////////////////////

require_once("viewopt.inc.php");        // required for VIEWOPT_ definitions
assert(isset($view) and get_class($view) == "ViewOptAndPos");
?>

<?php if ($showviewform) { ?>
<div style="line-height: 2.5em; width: 100%;">       <!-- this is required for valid XML -->
    <table id="viewmodetable" style="width: 100%">
    <tr><td>
    <p>

        <!-- viewmode box -->
        <strong> &gt; Show in</strong>
        <select name="viewmode" <?php writeAutoSubmit(); ?> >
               <?php
                    /*writeOptionItem("compact", "compact", $viewmode);*/
                    writeOptionItem(VIEWOPT_VIEWMODE_FULL, "full", $view->opt->viewmode);
                    writeOptionItem(VIEWOPT_VIEWMODE_SINGLETABLE, "single table", $view->opt->viewmode);
               ?>
        </select><strong> mode</strong><br/>

        <!-- browseby box -->
        <strong> &gt; Browse by</strong>
        <select name="browseby" <?php writeAutoSubmit(); ?> >
               <?php
                    writeOptionItem(VIEWOPT_BROWSEBY_NAME, "name", $view->opt->browseby);
                    writeOptionItem(VIEWOPT_BROWSEBY_CATEGORY, "category", $view->opt->browseby);
               ?>
        </select><br/>

        <!-- compperpage box -->
        <strong> &gt; Show
        <select name="compperpage" <?php writeAutoSubmit(); ?> >
               <?php
                    writeOptionItem(VIEWOPT_COMPPERPAGE_PARTIAL, "10", $view->opt->compperpage);
                    writeOptionItem(VIEWOPT_COMPPERPAGE_ALL, "all", $view->opt->compperpage);
               ?>
        </select>
        components per page</strong><br/>
    </p>
    </td>

    <!-- empty column for spacing purpose -->
    <td>&nbsp;</td>

    <?php
        if (!isset($includeviewbtn) || $includeviewbtn) {
    ?>
        <td style="width: 30%; vertical-align: middle">
            <input id="mybtn" style="height: 3em;" type="submit" name="submit" value="Change view mode"/>
        </td>
    <?php
        }
    ?>
    </tr></table>
</div>
<?php
     }
?>