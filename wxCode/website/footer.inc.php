<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 30/6/2005
  // RCS-ID: $Id: footer.inc.php,v 1.5 2006/05/15 13:35:31 frm Exp $
  // Notes: this file is included just before the </html> tag by all the
  //        files of the wxCode website. This file contains the standard
  //        wxCode footer.
  ////////////////////////////////////////////////////////////////////////////////////////////
?>
        <!-- ##### Footer ##### -->
        <div class="footer">

          <!-- Sourceforge logo (must be taken from SF servers; cannot be 'cached' in a local dir !) -->
          <div class="footerLeft">
            <a href="http://sourceforge.net">
                <img src="http://sourceforge.net/sflogo.php?group_id=51305&amp;type=4"
                            alt="SourceForge.net Logo" /></a>
          </div>

          <div class="footerRight">

            <!-- Valid XHTML logo -->
            <a href="http://validator.w3.org/check?uri=referer">
                <img src="<?php echo SITEBASEURL; ?>images/valid-xhtml10.png" alt="Valid XHTML 1.0!" /></a>
            <a href="http://jigsaw.w3.org/css-validator/check/referer">
                <img src="<?php echo SITEBASEURL; ?>images/vcss.png" alt="Valid CSS!"/></a>
          </div>
        </div>
    </div>
</body>
