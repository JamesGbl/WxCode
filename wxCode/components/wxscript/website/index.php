<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 7/7/2005
  // RCS-ID: $Id$
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
	<?php       
      $compname = "wxScript";
      $subdir = "wxscript";
      require("compwebsite.inc.php");
  ?>



<br/><h3>wxScript usage sample</h3>

<?php
global $BEAUT_PATH;
$BEAUT_PATH= "/home/groups/w/wx/wxcode/htdocs/wiki/cookbook/beautifier/php";
require_once("cookbook/beautifier/php/Beautifier/Init.php");
require_once "$BEAUT_PATH/HFile/HFile_cpp.php";
require_once "$BEAUT_PATH/Output/Output_HTML.php";
$highlighter = new Core(new HFile_cpp(), new Output_HTML());
print $highlighter->highlight_text(
'
// load the script
wxString filename(basepath + wxT("myscript"));
wxScriptFile *pf = wxScriptInterpreter::Load(filename, wxRECOGNIZE_FROM_EXTENSION);
if (pf == NULL) {
    wxPrintf(wxT(">Failed to load \'%s\'.\n"), filename.c_str());
    return;
}

// get the list of the functions
wxScriptFunctionArray arr;
wxScriptInterpreter::GetTotalFunctionList(arr);

// now, check for the presence of the "main" function...
wxScriptFunction *func = arr.Get(wxT("main"));
if (func == NULL) return; // no such function is present ?

// ...and run it with a string as argument
wxScriptVar result;
wxScriptVar args[2];
args[0].Set(wxT("char*"), wxT("my string"));
if (!func->Exec(result, args)) return;

// last, get the result...
wxString str(result.GetContentString());
'
);
?>


  <br/><?php write_h1("Documentation"); ?>
  <p>The doxygen-generated documentation for this component is available online
  <a href="../../docs/keybinder/">here</a>. Please let me know if you think something is missing.</p>
  

	<br/><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="../../faq.php">wxCode FAQ</a>.</p>

    
	<br/><?php write_h1("Future enhancements"); ?>
  <p>None. To submit a feature request please look at <a href="../../faq.php">wxCode FAQ</a>.</p>
    

	<br/><?php require("footer.inc.php"); ?>
</html>
