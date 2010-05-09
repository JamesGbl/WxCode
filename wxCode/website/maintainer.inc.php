<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 3/7/2005
// RCS-ID: $Id: maintainer.inc.php,v 1.1 2005/09/03 23:14:52 frm Exp $
// Notes: the XHTML code for the maintainer info form
////////////////////////////////////////////////////////////////////////////////////////////

require_once("comp.inc.php");

// look for maintainer data
if (!isset($maint))
    $maint = new Maintainer();
$maint->fill_gaps();
          
if ($loadfrompost == TRUE)
    $maint->get_from_post();
?>

<p>Please enter the following <strong>basic info about you</strong>: your name (first name, last name), your correct e-mail address (<strong>do not include something like @nospam in the mail address</strong> since it will be automatically added when your address is shown in the component list) and the user name of your SourceForge.net account.</p>

<p>You must also choose a <strong>password</strong> that will be used to access the edit page of your component info stored in the wxCode database. Some important notes about this: this password is stored in encrypted form in the database and thus if you lost it noone will be able to recover it. This also means that <strong><em>you could also use your SourceForge account password</em></strong> to avoid to be forced to remember yet another password... obviously this is the unadvised way since it's a good habit use a different password for each different site/service.</p>

<table cellpadding="2" cellspacing="4" class="submitFormTable">
  <tr>
    <td>Maintainer name:</td>
    <td><input name="maintainername" type="text" maxlength="32" value="<?php echo "$maint->name"; ?>"/></td>
    <td style="width: 5%;"/>
    <td>Maintainer email address:</td>
    <td><input name="maintainermail" type="text" maxlength="32" value="<?php echo "$maint->mail"; ?>"/></td>
  </tr>
  <tr>
    <td>Maintainer SF account:</td>
    <td><input name="maintainersfuser" type="text" maxlength="32" value="<?php echo "$maint->sfuser"; ?>"/></td>
    <td/>
    <td>Maintainer password:</td>
    <td><input name="maintainerpassword" type="password" maxlength="64" value="<?php echo "$maint->password"; ?>"/></td>
  </tr>
</table>

<?php if (isset($includesendbtn) and $includesendbtn) { ?>
<p>When you have filled in all fields you may submit them.</p>
<div style="text-align: center; margin-left: auto; margin-right: auto;">
<input style="width: 30%; height: 2em;" type="submit" name="maintFormBtn" value="Send"/>
</div>
<?php } ?>

