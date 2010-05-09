// Author:  Paul Sowden, Francesco Montorsi
// RCS-ID:  $Id: styleswitcher.js,v 1.11 2005/11/07 22:31:53 frm Exp $
// Purpose: JavaScript for changing the active stylesheet for XHTML pages
//          (taken from http://www.alistapart.com/articles/alternate/)


function onCSSselector(todisableID, obj) {

    // set the current CSS to use
    setActiveStyleSheet(obj.id);
    
    // mark the current link as the "active one"
    obj.style.color = 'yellow';

    // remove any previous angle brackets, if they exist
    obj.firstChild.nodeValue = obj.firstChild.nodeValue.replace("> ", "");
    obj.firstChild.nodeValue = obj.firstChild.nodeValue.replace(" <", "");
    obj.firstChild.nodeValue = "> " + obj.firstChild.nodeValue + " <";
    
    // mark as "not active" the old link
    var other = document.getElementById(todisableID);
    if (other) {
        other.style.color = 'white';
        
        // remove the angle brackets, if they exist
        other.firstChild.nodeValue = other.firstChild.nodeValue.replace("> ", "");
        other.firstChild.nodeValue = other.firstChild.nodeValue.replace(" <", "");
    }
    
    return false;
}


function setActiveStyleSheet(title) {
  var i, a, main;
  for(i=0; (a = document.getElementsByTagName("link")[i]); i++) {
    if(a.getAttribute("rel").indexOf("style") != -1 && a.getAttribute("title")) {
      a.disabled = true;
      if(a.getAttribute("title") == title)
        a.disabled = false;
    }
  }
}

function getActiveStyleSheet() {
  var i, a;
  for(i=0; (a = document.getElementsByTagName("link")[i]); i++) {
    if(a.getAttribute("rel").indexOf("style") != -1 && a.getAttribute("title") && !a.disabled) return a.getAttribute("title");
  }
  return null;
}

function getPreferredStyleSheet() {
  var i, a;
  for(i=0; (a = document.getElementsByTagName("link")[i]); i++) {
    if(a.getAttribute("rel").indexOf("style") != -1
       && a.getAttribute("rel").indexOf("alt") == -1
       && a.getAttribute("title")
       ) return a.getAttribute("title");
  }
  return null;
}

function createCookie(name,value,days) {
  if (days) {
    var date = new Date();
    date.setTime(date.getTime()+(days*24*60*60*1000));
    var expires = "; expires="+date.toGMTString();
  }
  else expires = "";
  document.cookie = name+"="+value+expires+"; path=/";
}

function readCookie(name) {
  var nameEQ = name + "=";
  var ca = document.cookie.split(';');
  for(var i=0;i < ca.length;i++) {
    var c = ca[i];
    while (c.charAt(0)==' ') c = c.substring(1,c.length);
    if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
  }
  return null;
}

window.onload = function(e) {
  var cookie = readCookie("style");
  var title = cookie ? cookie : getPreferredStyleSheet();
  setActiveStyleSheet(title);
}

window.onunload = function(e) {
  var title = getActiveStyleSheet();
  createCookie("style", title, 365);
}


// this code is immediately executed 
var cookie = readCookie("style");
var title = cookie ? cookie : getPreferredStyleSheet();

// we pass NULL as first argument since we are sure that there are no
// old CSS selector tags to mark as inactive and then we search the
// CSS selector tag whose ID is the same as the CSS it selects when clicked
onCSSselector(null, document.getElementById(title));
