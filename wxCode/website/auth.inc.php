<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 5/7/2005
// RCS-ID: $Id: auth.inc.php,v 1.7 2005/09/03 23:14:52 frm Exp $
// Notes: contains the XHTML code for the authentication form for a wxCode maintainer.
////////////////////////////////////////////////////////////////////////////////////////////
?>

<div>

<!-- the page the user tried to access is hardcoded here as the EDIT page;
     this is because it's the only page that currently requires authentication -->
<p>The access to the &quot;edit component info&quot; page is restricted to wxCode maintainers only.</p>
<table cellpadding="2" cellspacing="4" class="submitFormTable">
  <tr>
    <td style="width: 25%;">Maintainer SF account:</td>
    <td style="width: 25%;"><input name="user" type="text" maxlength="32" value=""/></td>
    <td style="width: 50%;"/>
  </tr>
  <tr>
    <td>Maintainer password:</td>
    <td><input name="pass" type="password" maxlength="32" value=""/></td>
    <td/>
  </tr>
  <tr>
    <!-- <td>Remember me next time:</td> FIXME actually the non-remember-me feature does not work !! -->
    <td><input style="width:auto;" type="hidden" name="remember" checked="checked"/></td>
  </tr>
</table>

<br/>
<div style="text-align: center; margin-left: auto; margin-right: auto;">
<input style="width: 30%; height: 2em;" type="submit" name="sublogin" value="Log in"/>
</div>
</div>
