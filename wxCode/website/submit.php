<?php require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 30/6/2005
  // RCS-ID: $Id: submit.php,v 1.21 2005/09/03 23:14:52 frm Exp $
  // Notes: allows the user to ask for submission of its own component.
  //        Since this page handles both the submission forms (one for the maintainer
  //        entry in the "users" table and one for the entry in the "components" table),
  //        and the insertion of the data in the database,
  //        this file is able to generate completely different pages based on the values
  //        available in the $_POST array.
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="submit";
        $menuentry="Submit your component";
        $pagetitle="How to submit your component to wxCode";
        require("header.inc.php");
    ?>

    <?php write_h1("How to submit your component to wxCode", "submit", FALSE); ?>
    <p>If you want to apply for submission of a new wxWidgets-based component you must, in order:</p>
    <ol>
    <li><strong>Read and agree the <a href="rules.php">wxCode hosting rules</a></strong>.</li>
    <li>If you are not already registered as wxCode maintainer (have you ever submitted a component to wxCode ?), then you must <strong>register as wxCode maintainer</strong> using <a href="register.php">this form</a>.</li>
    <li>Start filling out <strong>all</strong> the fields of the <a href="compsubmit.php">component submission form</a>. The description is <strong>not</strong> optional (like all other fields): in fact, it is always the most characterizing information about a component. Write down clearly and concisely the most important things about your code, without repeating the information available in the dedicated fields (like wxWidgets's supported versions and ports, the presence of the documentation, the status...).</li>
    <li>Wait: this submission will generate a message in wxcode-users mailing list where you can check your entered values and where <strong>your submission will be checked by wxCode administrators</strong>.
    You will receive as soon as possible a reply from wxCode administrators...</li>
    <li>Look at the &quot;<strong><a href="maintguide.php">wxCode maintainer guide</a></strong>&quot; to learn how
    to access the SVN repository of your component; how to make a new release for your component, how to create your component
    website, etc...</li>
    </ol>
    
    <br/><p>Note that if you don't want to submit a new component but instead you want to become a maintainer for
    a currently unmaintained component, then you should look at the <a href="help.php">help page</a>...</p>

    <br/><?php require("footer.inc.php"); ?>
</html>
