#!/bin/sh
#
# Author: Francesco Montorsi
# RCS-ID: $Id$
# Creation date: 14/9/2005
#
# A simple script to generate the configure script for a wxCode component


( bakefile_gen && aclocal && autoconf && mv configure .. )
