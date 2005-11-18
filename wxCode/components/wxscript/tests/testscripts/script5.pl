#!/usr/bin/perl
# Script5.pl - a sample Perl script
#//////////////////////////////////////////////////////////

use warnings;
use strict;
#use Win32;



sub pl_func1($$)
{
    my $str = $_[0];
    my $n = $_[1];
    my $res = 0;
    my $cloop;
    my $c;

    #Win32::MsgBox("pl_func1 called", 48, "func1");
    
    for ($cloop = 0; $cloop < $n; $cloop++) {
        $c = substr $str, $cloop, 1;
        $res += ord $c;
    }
    
    return ($res);
}

sub pl_func2($)
{
    #Win32::MsgBox("pl_func2 called", 48, "func2");
	return (not $_[0]);
}

#Win32::MsgBox("script5.pl Loaded", 48, "Load");
#print (pl_func1("test", 3), "\n");
#print (pl_func2(0), "\n");
