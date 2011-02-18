<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 3/7/2005
// RCS-ID: $Id: form.inc.php,v 1.46 2009/04/05 11:50:38 frm Exp $
// Notes: contains the XHTML code for the submission, edit, search form of a wxCode component.
//        before including this file include the "globals.php" file...
////////////////////////////////////////////////////////////////////////////////////////////

// for the Component class
require_once("comp.inc.php");

// need a component to proceed...
if (!isset($comp))
    $comp = new Component();
if (!isset($maint))
    $maint = new Maintainer();

// eventually use defaults for those non-set values
$comp->fill_gaps();
$maint->fill_gaps();

if ($loadfromvar)
    $comp->get_from_var($var);

// button label
if (!isset($submitlabel))
  $submitlabel = "Submit";

// other labels
if (!isset($labels))
  $labels = "new";    // show the labels for a new insertion of a component.

// do we have to show the form ?
if (!isset($showcompform))
  $showcompform = TRUE;

function writeDisabledAttr($inputname)
{
    // use the single quotes so that $ symbols do not need to be escaped
    $disablestring = " disabled=\"disabled\"";
    eval('global $' . $inputname . 'disabled;');
    eval('if ($' . $inputname . 'disabled) echo $disablestring;');
}

function writeInputAttr($inputname)
{
    $inputvalue = "";
    $inputdisabled = FALSE;

    // use the single quotes so that $ symbols do not need to be escaped
    eval('global $comp, $' . $inputname . ', $' . $inputname . 'disabled;');
    eval('$inputvalue = $comp->' . $inputname . ';');
    eval('$inputdisabled = $' . $inputname . 'disabled;');

    echo "value='$inputvalue'";
    if ($inputdisabled)
        echo " disabled='disabled'";
}

function writeSearchItem($label, $var, $val = "%")
{
    global $searchmode;
    if ($searchmode == FALSE) return;

    writeOptionItem($val, $label, $var);
}

function writeRepoRadio($label)
{
    global $comp;

    // NOTE: the width:auto attribute is very important since our CSS
    //       sets it to 100% for <input> tags!
    echo "<input type='radio' style='width:auto' name='inCVS' ";

    $checked = FALSE;
    if ($label == "CVS")
    {
        echo "value='1' ";
        $checked = (bool)( $comp->inCVS );
    }
    else
    {
        echo "value='0' ";
        $checked = ! (bool)( $comp->inCVS );
    }

    if ($checked)
        echo "checked='checked' ";

    // note: we put the label after the <input> tag because we want it to
    //       be on the right of the radio button
    echo "/>$label";
}

function writeCat($catname, $value = "")
    // if value is empty, $catname will be used not only for label, also for value
{
    global $comp;

    // make first letter uppercase
    $nicename = strtoupper(substr($catname, 0, 1)) . substr($catname, 1);

    // this INPUT control must be inside a .submitFormTableWithCheckboxes style block !
    if ($value == "") $value = $catname;
    echo "<td><p><input name='category[]' type='checkbox' value='$value' ";

    // write the checked attribute;
    // NOTE: it's important not to write "checked='0'" when the item must be unchecked
    //       since the presence of the "checked" attribute (regardless of its value)
    //       will still be interpreted as a CHECKED status
    // NOTE2: the 'double negation' is required
    if (!is_null($comp->category)) {

        // the component category should always be stored as a string
        // (see Component class functions)
        assert(is_string($comp->category));
        $notfound = (strpos($comp->category, $value) === false);
        if (!$notfound)
            echo "checked='1'";
    }

    echo "/>$nicename</p></td>\n";      // the label must immediately follow the INPUT tag and ends with <br/>
}

function writeCategories()
{
    global $searchmode, $comp;

    // this for the search mode
    if ($searchmode) {
        echo "<tr>";
        echo "<!-- $comp->category -->";
        writeCat("Any", "%");
        echo "</tr>";
    }

    // these should be up2date with wxCode DB
    echo "<tr>";
    writeCat("window container");
    writeCat("dialog");
    writeCat("control");
    echo "</tr>\n<tr>";
    writeCat("window layout");
    writeCat("networking");
    writeCat("stream");
    echo "</tr>\n<tr>";
    writeCat("database");
    writeCat("data container");
    writeCat("graphics");
    echo "</tr>\n<tr>";
    writeCat("text processing");
    writeCat("printing");
    writeCat("import/export");
    echo "</tr>\n<tr>";
    writeCat("system");
    writeCat("tutorials/documentation");
    writeCat("application");
    echo "</tr>\n<tr>";
    writeCat("wrapper");
    writeCat("miscellaneous");
    echo "<td/></tr>";       // to complete the row
}

if ($showcompform) {
?>

    <!-- MAINTAINER BLOCK -->
    <?php if ($showmaintainersfuser) { ?>
    <p>First of all, you need to insert the SF account name of the maintainer of this component, and his password.
    Be sure that you <a href="register.php">registered as wxCode maintainer</a> or your submission won't be accepted.</p>
    <table cellpadding="2" cellspacing="4" class="submitFormTable">
      <tr>
        <td>Maintainer SF account:</td>
        <td><input name="maintainersfuser" type="text" maxlength="32"
                value="<?php echo "$maint->sfuser"; ?>"/></td>
        <td style="width: 50%;"/>
      </tr>
      <tr>
        <td>Maintainer password:</td>
        <td><input name="maintainerpassword" type="password" maxlength="32"
                value="<?php echo "$maint->password"; ?>"/></td>
        <td style="width: 50%;"/>
      </tr>
    </table>
    <?php } ?>

    <!-- COMPONENT INFO BLOCK -->
    <?php if ($labels == "new") { ?>
    <p>Now, some <strong>basic informations about the component</strong>; remember that component name can be freely chosen but it has to consist of only lowercase letters, has to be unique within wxCode and must not contain spaces. Please don't use dashes or underscores.</p>

    <p>Also the &quot;Subdir&quot; must be a valid unix file name and must consist of lowercase letters and digits only and should in some way match the component name. It has to be unique within all the components of wxCode and it <strong>should not start with the 'wx' prefix</strong>.</p>

    <p>Starting in February 2011 all wxCode components reside in a <a href="http://en.wikipedia.org/wiki/Subversion_%28software%29">Subversion (SVN)</a> repository. It is no longer possible to opt for the <a href="http://en.wikipedia.org/wiki/Concurrent_Versions_System">Concurrent Versioning System (CVS)</a>.</p>

    <p>The &quot;Component wiki&quot; field allows you to choose if you want to keep a wiki dedicated for this component as the component website (when using value '<em>as website</em>'), if you want to keep the wiki '<em>enabled</em>' but separated from the actual component website (which you have to create in the SVN using the given XHTML template...) or if you do not want any wiki for this component (in this case select '<em>disabled</em>').</p>
    <?php } ?>

    <?php if ($labels == "new") { ?>
    <p>Finally, note that this form allows you to specify an &quot;Author name&quot; which can be
    different from the maintainer's one since you could be submitting a component which was originally
    created by someone else...</p>
    <?php } ?>

    <table cellpadding="2" cellspacing="4" class="submitFormTable">
      <tr>
        <td>Component name:</td>
        <td><input name="name" type="text" maxlength="32" <?php writeInputAttr("name"); ?> /></td>
        <td style="width: 5%;"/>
        <td>Component creation date<br/>(year/month/day):</td>
        <td><input name="cdate" type="text" maxlength="32" <?php writeInputAttr("cdate"); ?> /></td>
      </tr>
      <tr>
        <td>Subdir:</td>
        <td><input name="location" type="text" maxlength="72" <?php writeInputAttr("location"); ?> /></td>
        <td/>
        <td>Repository:</td>
        <td>
            <?php
//                writeRepoRadio("CVS");
//                echo "\n&nbsp;&nbsp;&nbsp;\n";
//                writeRepoRadio("Subversion");
                echo "<input type=\"hidden\" name=\"inCVS\" value=\"0\"/>Subversion";
            ?>
        </td>
      </tr>
    </table>

    <!-- this block is shown both when submitting a new component and when editing an existing one -->
    <p>To let users find easily your component, you should carefully set the following category fields; use the <a href="categories.php">wxCode categories page</a> as reference. They are very important since the current component list opens automatically in the &quot;browse by category&quot; mode. It's also important <strong>not to abuse of the &quot;Miscellaneous&quot; category</strong>. If you want to specify a single category for your component then set both category fields to the same value.</p>
    <?php if ($labels == "edit") { ?>
    <p>Also note that if you re-categorize your component, your changes won't be immediately reflected in the <a href="complist.php">component list page</a> (unlike all other changes you can make in this page) but will take up to 24 hours to update...</p>
    <?php } ?>

    <!-- categories table -->
    <p><td/></p>
    <table cellpadding="2" cellspacing="4" class="submitFormTableWithCheckboxes ">
      <tr>
        <td>Categories:</td>
        <td/>
        <td/>
      </tr>
      <?php writeCategories($comp->category); ?>
    </table>
    <p><td/></p>

    <table cellpadding="2" cellspacing="4" class="submitFormTable">
      <tr>
        <td>External dependencies:</td>
        <td><input name="extdep" type="text" maxlength="72" value="<?php echo "$comp->extdep"; ?>"/></td>
        <td/>
        <td>Programming language:</td>
        <td><select name="language">
            <?php
                writeOptionItem("cpp", "C++", $comp->language);
                writeOptionItem("wxlua", "wxLua", $comp->language);
                writeOptionItem("wxpython", "wxPython", $comp->language);
                writeOptionItem("wxperl", "wxPerl", $comp->language);
                writeOptionItem("wxbasic", "wxBasic", $comp->language);
                writeOptionItem("wxruby", "wxRuby", $comp->language);
                writeSearchItem("Any", $comp->language);
             ?>
        </select></td>
      </tr>
      <tr>
        <td>Component wiki:</td>
        <td><select name="wiki" <?php writeDisabledAttr("wiki"); ?> >
            <?php
                writeOptionItem("website", "As website", $comp->wiki);
                writeOptionItem("enabled", "Enabled", $comp->wiki);
                writeOptionItem("disabled", "Disabled", $comp->wiki);
                writeSearchItem("Any", $comp->wiki);
             ?>
        </select></td>
        <td/>
        <td>Author name:</td>
        <td><input name="author" type="text" maxlength="32" value="<?php echo "$comp->author"; ?>"/></td>
      </tr>
    </table>

    <?php if ($labels == "new") { ?>
    <p>Now add a brief <strong>description of your component</strong>. It should be enough so any user understands <strong>what your component does</strong>. Don't write <em>how</em> it does since that belongs to the component docs. A few lines here should be sufficient.</p>
    <?php
          } else {

              // inside FORM elements <br/> tags are not allowed so we enclose it in <p> tags
              echo "<p><br/></p>";
          }
    ?>

    <table cellpadding="2" cellspacing="4" class="submitFormTable">
      <tr><td>Description:</td></tr>
      <tr>
        <td>
        <!-- the ROWS and COLS attributes are required ! -->
        <textarea style="width:100%; height:8em;" rows="10" cols="72" name="description"><?php echo "$comp->description"; ?></textarea>
        </td>
      </tr>
    </table>

    <?php if ($labels == "new") { ?>
    <p>Now, submit some <strong>additional info about the current status of the component</strong>.<br/>
    In the &quot;Version&quot; field you can use any versioning system you like (typically something like &quot;major.minor&quot; should be enough for a component; but you can also opt for a simple date-system: i.e. you keep this field updated with the date of the latest available release for your component).</p>
    <?php
          } else {

              // inside FORM elements <br/> tags are not allowed so we enclose it in <p> tags
              echo "<p><br/></p>";
          }
    ?>

    <table cellpadding="2" cellspacing="4" class="submitFormTable">
      <tr>
        <td>Status:</td>
        <td><select name="status">
            <?php
                writeOptionItem("alpha", "Alpha", $comp->status);
                writeOptionItem("beta", "Beta", $comp->status);
                writeOptionItem("stable", "Stable", $comp->status);
                writeSearchItem("UNMAINTAINED", $comp->status, "UNMAINTAINED");
                writeSearchItem("Any", $comp->status);
             ?>
        </select></td>
        <td style="width: 5%;"/>
        <td>Version:</td>
        <td><input name="version" type="text" maxlength="32" value="<?php echo "$comp->version"; ?>"/></td>
      </tr>
      <tr>
        <td>Number of samples provided:</td>
        <td><input name="samples" type="text" maxlength="32" <?php writeInputAttr("samples"); ?> /></td>
        <td/>
        <td>Documentation:</td>
        <td><select name="docs">
            <?php
                writeOptionItem("doxygen", "Doxygen", $comp->docs);
                writeOptionItem("hand", "Written by hand", $comp->docs);
                writeOptionItem("notavailable", "Not provided", $comp->docs);
                writeSearchItem("Any", $comp->docs);
             ?>
        </select></td>
      </tr>
    </table>


    <!--
        PHP NOTE: as these controls have the MULTIPLE selection attribute set,
                   their names must end with the square brackets [] to be able
                   to get all their values in the PHP scripts.
                   See http://www.faqts.com/knowledge_base/view.phtml/aid/3966
    -->
    <p>In the following controls you can make <strong>multiple selections</strong> (typically using the CTRL button when clicking):</p>
    <table cellpadding="2" cellspacing="4" class="submitFormTable">
      <tr>
        <td>wxWidgets supported port(s):<br/>
            <span style="font-weight: normal; text-align:center;">
                (this is very important;<br/> select at least one entry)
            </span></td>
        <td><select name="wxport[]" multiple="multiple">
            <?php
                writeOptionItem("wxmsw", "wxMSW", $comp->wxport);
                writeOptionItem("wxgtk", "wxGTK", $comp->wxport);
                writeOptionItem("wxx11", "wxX11", $comp->wxport);
                writeOptionItem("wxmac", "wxMac", $comp->wxport);
                writeOptionItem("wxmotif", "wxMotif", $comp->wxport);
                writeOptionItem("all", "All", $comp->wxport);
                writeSearchItem("Any", $comp->wxport);
             ?>
        </select></td>
        <td/>
        <td>wxWidgets supported version(s):<br/>
            <span style="font-weight: normal; text-align:center;">
                (this is very important;<br/> select at least one entry)
            </span></td>
        <td><select name="wxversion[]" multiple="multiple">
            <?php
                writeOptionItem("2.4", "2.4.x", $comp->wxversion);
                writeOptionItem("2.5", "2.5.x", $comp->wxversion);
                writeOptionItem("2.6", "2.6.x", $comp->wxversion);
                writeOptionItem("2.7", "2.7.x", $comp->wxversion);
                writeOptionItem("2.8", "2.8.x", $comp->wxversion);
                writeOptionItem("2.9", "2.9.x", $comp->wxversion);
                writeOptionItem("cvs", "CVS", $comp->wxversion);
                writeOptionItem("all", "All", $comp->wxversion);
                writeSearchItem("Any", $comp->wxversion);
             ?>
        </select></td>
      </tr>
      <tr>
        <td>Build system:<br/>
            <span style="font-weight: normal; text-align:center;">
                (deselect all options if you do<br/> not provide any build system)
            </span>
        </td>
        <td><select name="buildsys[]" multiple="multiple">
            <?php
                writeOptionItem("bakefile", "Bakefile-based", $comp->buildsys);
                writeOptionItem("cmake", "CMake", $comp->buildsys);
                writeOptionItem("makefiles", "Makefiles", $comp->buildsys);
                writeOptionItem("projectfiles", "Project files", $comp->buildsys);
                writeSearchItem("Any", $comp->buildsys);
             ?>
        </select></td>

      </tr>
    </table>

    <?php if (isset($includesendbtn) and $includesendbtn) { ?>
    <p>When you have filled in all fields you may submit them.</p>
    <div style="text-align: center; margin-left: auto; margin-right: auto;">
    <input style="width: 30%; height: 2em;" type="submit" name="compFormBtn"
            value="<?php echo $submitlabel; ?>"/>
    </div>
    <?php } ?>
<?php } /* if($showviewform) */ ?>
