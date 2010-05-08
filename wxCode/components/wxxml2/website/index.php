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
      $compname = "wxXml2";
      $subdir = "wxxml2";
      require("compwebsite.inc.php");
    ?>


    <!-- NOTE ON THE SECTIONS TO INSERT HERE: you are free to add anything you think
       that will make your component more appealing: some sample snippet of code,
       docs, bugs & feature requests... -->
    <br/><?php write_h1("Usage sample"); ?>
    <br/><?php write_h1("Usage sample"); ?>
    <?php writeSnippet('
    // parse an XML file  
    wxXml2Document doc;
    if (!doc.Load("myfile.xml"))
        return false;

    // start processing the XML file
    if (doc.GetRoot()->GetName() != "myroot-node")
        return false;

    wxXml2Node *child = doc.GetRoot()->GetChildren();
    while (child) {

        if (child->GetName() == "tag1") {

            // process text enclosed by tag1/tag1
            wxString content = child->GetNodeContent();

            ...

            // process attributes of tag1
            wxString attrvalue1 =
                child->GetProperty("attr1", "default-value");
            wxString attrvalue2 =
                child->GetProperty("attr2", "default-value");

            ...

        } else if (child->GetName() == "tag2") {

            // process tag2 ...
        }

        child = child->GetNext();
    }
    '); ?>



    <br/><?php write_h1("Screenshots"); ?>
    <p>Obviously, since wxXml2 is not an application but just a wrapper for the libxml2 library,
    the following screenshots refers to the sample app included with this module which shows
    how much easy is to get the tree structure of an XML doc through wxXml2 classes:</p><br>
    <div align="center">
    <?php writeScreenshotLink("screenshot.png", "The wxXml2 sample running on Windows XP"); ?>
    &nbsp;&nbsp;&nbsp;&nbsp;
    <?php writeScreenshotLink("screenshot2.png", "The wxXml2 sample running on Red Hat 9"); ?>
    </div>


    <br/><?php write_h1("Documentation"); ?>
    <p>The doxygen-generated documentation for this component is available online
    <a href="../../docs/wxxml2/">here</a>. Please let me know if you think something is missing.</p>


    <br/><?php write_h1("Known bugs"); ?>
    <p>None. To submit a bug report please look at <a href="../../faq.php">wxCode FAQ</a>.</p>


    <br/><?php write_h1("Future enhancements"); ?>
    <p>Support for libxsl.<br/>To submit a feature request please look at <a href="../../faq.php">wxCode FAQ</a>.</p>


    <br/><?php require("footer.inc.php"); ?>
</html>
