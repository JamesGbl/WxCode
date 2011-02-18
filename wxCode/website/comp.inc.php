<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 3/9/2005
// RCS-ID: $Id: comp.inc.php,v 1.12 2009/04/05 11:50:38 frm Exp $
// Notes: defines the Component class and all its helpers
////////////////////////////////////////////////////////////////////////////////////////////


class Component {

    // strings
    var $name, $description, $location, $extdep, $author, $version;

    // SETs (stored as comma-separed list: this makes easier to write things like:
    //       writeOptionItem("bakefile", "Bakefile-based", $comp->buildsys);  )
    var $category, $wxversion, $buildsys, $wxport;

    // ENUMs
    var $language, $status, $docs, $wiki;

    // numbers
    var $id, $samples, $approved, $maintainerid;

    // dates
    var $cdate;

    // booleans
    var $inCVS;


    function check_set_array(& $tocheck)
    {
        if (is_null($tocheck))
            return;

        if (!is_string($tocheck)) {
            assert(is_array($tocheck));
            $tocheck = convertArrayToStr($tocheck);
        }
    }

    function get_from_post()
    {
        $this->get_from_var($_POST);
    }

    function get_from_var($arrvar)
    {
        // get plain data
        $this->name = $arrvar['name'];
        $this->description = $arrvar['description'];
        $this->location = $arrvar['location'];
        $this->extdep = $arrvar['extdep'];
        $this->author = $arrvar['author'];
        $this->version = $arrvar['version'];

        // sets
        $this->buildsys = $arrvar['buildsys'];
        $this->wxversion = $arrvar['wxversion'];
        $this->wxport = $arrvar['wxport'];
        $this->category = $arrvar['category'];

        // enums
        $this->language = $arrvar['language'];
        $this->status = $arrvar['status'];
        $this->docs = $arrvar['docs'];
        $this->wiki = $arrvar['wiki'];

        // numbers
        $this->id = $arrvar['id'];
        $this->samples = $arrvar['samples'];
        $this->approved = $arrvar['approved'];
        $this->maintainerid = $arrvar['maintainerid'];

        // dates
        $this->cdate = $arrvar['cdate'];

        // booleans
//        $this->inCVS = (bool)( $arrvar['inCVS'] );
        $this->inCVS = FALSE;

        $this->check_set_array($this->category);
        $this->check_set_array($this->buildsys);
        $this->check_set_array($this->wxversion);
        $this->check_set_array($this->wxport);

        echo "\n<!--\n";
        var_dump($this);
        echo "-->\n\n";
    }

    function get_from_db($name)
    {
        global $dbconn;
        db_connect();  // be sure to be connected

        // do the query
        $result = mysql_query("SELECT * FROM components WHERE name='$name'", $dbconn);
        $var = mysql_fetch_assoc($result);

        // convert to our internal format...
        $this->get_from_var($var);
    }

    function updatedb(& $query)
    {
        // update the component except for the NAME & APPROVED columns (which cannot be modified
        // even by a wxCode maintainer) getting all the data from the _POST array
        global $dbconn;
        db_connect();   // be sure to be connected

        $query = "UPDATE components SET wxversion='$this->wxversion', category='$this->category', "
                    . "language='$this->language', description='$this->description', "
                    . "location='$this->location', cdate='$this->cdate', status='$this->status', "
                    . "docs='$this->docs', buildsys='$this->buildsys', extdep='$this->extdep', "
                    . "wiki='$this->wiki', wxport='$this->wxport', samples='$this->samples', "
                    . "author='$this->author', version='$this->version', inCVS='$this->inCVS' "
                    . "WHERE name='$this->name' LIMIT 1";
        return mysql_query($query, $dbconn);
    }

    function insert_in_db()
    {
        global $dbconn;
        db_connect();   // be sure to be connected

        $res = mysql_query("INSERT INTO components SET name='$this->name', wxversion='$this->wxversion', "
                . "category='$this->category', language='$this->language', description='$this->description', "
                . "location='$this->location', cdate='$this->cdate', status='$this->status', docs='$this->docs', "
                . "buildsys='$this->buildsys', extdep='$this->extdep', wiki='$this->wiki', wxport='$this->wxport', "
                . "samples='$this->samples', approved='0', author='$this->author', version='$this->version', "
                . "inCVS='$this->inCVS', maintainerid='$this->maintainerid'", $dbconn);
        if ($res == FALSE)
            return FALSE;
        return TRUE;
    }

    function fill_gaps()
    {
        // strings
        if (!isset($this->author)) $this->author="";
        if (!isset($this->name)) $this->name="";
        if (!isset($this->extdep)) $this->extdep="none";
        if (!isset($this->location)) $this->location="the name just in lowercase";
        if (!isset($this->description)) $this->description="Put here 5-6 lines of description...";
        if (!isset($this->version)) $this->version="1.0";

        // dates
        if (!isset($this->cdate)) $this->cdate=date("Y-m-d");

        // enums
        if (!isset($this->language)) $this->language="C++";
        if (!isset($this->wiki)) $this->wiki="disabled";
        if (!isset($this->status)) $this->status="alpha";
        if (!isset($this->docs)) $this->docs="notavailable";

        // numbers
        if (!isset($this->samples)) $this->samples="0";

        // SETs
        if (!isset($this->category)) $this->category="";
        if (!isset($this->wxport)) $this->wxport="all";
        if (!isset($this->wxversion)) $this->wxversion="all";
        if (!isset($this->buildsys)) $this->buildsys="notavailable";

        // booleans
        if (!isset($this->inCVS)) $this->inCVS=FALSE;
    }

    function init_all_with($val)
    {
        assert(is_string($val));
        $this->name = $val;
        $this->author = $val;
        $this->language = $val;
        $this->extdep = $val;
        $this->description = $val;
        $this->status = $val;
        $this->version = $val;
        $this->docs = $val;
        $this->inCVS = $val;

        // use $val string even for SETS (which are stored as comma-separed *strings*)
        $this->category = $val;
        $this->wxport = $val;
        $this->wxversion = $val;
        $this->buildsys = $val;
    }

    function are_required_fields_valid()
    {
        if ($this->name == "" or $this->location == "" or
            $this->description == "" or $this->author == "")
            return FALSE;

        if (!isStrValid($this->name) or !isStrLowercase($this->location))
            return FALSE;

        return TRUE;
    }

    function check_for_duplicates()
    {
            // begin process to insert this new component in the database
            global $dbconn;
            db_connect();       // be sure to be connected

            // but first check there are no other components named the same
            $res = mysql_query("SELECT * FROM components WHERE "
                    . "(name LIKE '$this->name' OR location LIKE '$this->location')", $dbconn);
            if ($res == FALSE or mysql_num_rows($res) != 0)
                return TRUE;
         return FALSE;
    }
}

class Maintainer {

    // NOTE: approved is now unused
    var $id, $mail, $sfuser, $name, $password, $approved;

    function insert_in_db()
    {
        global $dbconn;
        db_connect();   // be sure to be connected

        // we can now insert this new field
        $res = mysql_query("INSERT INTO maintainers SET sfuser='$this->sfuser', "
                        . "password=MD5('$this->password'), name='$this->name', "
                        . "mail='$this->mail'", $dbconn);
        if ($res == FALSE)
            return FALSE;
        return TRUE;
    }

    function get_from_db($sfuser)
    {
        global $dbconn;
        db_connect();       // be sure to be connected

        $res = mysql_query("SELECT * FROM maintainers WHERE sfuser='$sfuser'", $dbconn);
        if ($res == FALSE or mysql_num_rows($res) == 0)
            return FALSE;

        // ok we now have a valid maintainer ID
        $row = mysql_fetch_assoc($res);
        $this->get_from_var($row);

        return TRUE;
    }

    function get_from_var($row)
    {
        $this->id = $row['id'];
        $this->mail = $row['mail'];
        $this->sfuser = $row['sfuser'];
        $this->name = $row['name'];
        $this->password = $row['password'];
        $this->approved = $row['approved'];
    }

    function get_from_post()
    {
        // when sending maintainer data through forms we prepend 'maintainer' to the varname to
        // avoid conflicts with component data (like for 'name' var)
        $this->id = $_REQUEST['maintainerid'];
        $this->mail = $_REQUEST['maintainermail'];
        $this->sfuser = $_REQUEST['maintainersfuser'];
        $this->name = $_REQUEST['maintainername'];
        $this->password = $_REQUEST['maintainerpassword'];
        $this->approved = $_REQUEST['maintainerapproved'];

        echo "\n<!--\n";
        var_dump($this);
        echo "-->\n\n";
    }

    function fill_gaps()
    {
        if (!isset($this->name)) $this->name="";
        if (!isset($this->sfuser)) $this->sfuser="";
        if (!isset($this->password)) $this->password="";
        if (!isset($this->mail))
            if ($this->sfuser == "")
              $this->mail="";
            else
              $this->mail=$this->sfuser . "@users.sourceforge.net";
    }

    // Returns TRUE if a maintainer with same SFUSERNAME or same NAME already exist in the DB
    function check_for_duplicates()
    {
            // begin maintainer registration process
            global $dbconn;
            db_connect();       // be sure to be connected

            // first check there are no other maintainers named the same
            $res = mysql_query("SELECT * FROM maintainers WHERE ( sfuser='$this->sfuser' OR name='$this->name' )", $dbconn);

            if ($res == FALSE or mysql_num_rows($res) != 0)
                return TRUE;
         return FALSE;
    }
}


?>
