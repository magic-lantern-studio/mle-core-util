# Magic Lantern Utility Library

The Magic Lantern Core Utility Library is a common library that may be
used in both Magic Lantern tools as well as target platform titles.

## Overview

The utility library is comprised of software that is common across
Magic Lantern tools and titles. It provides code to load shared libraries
(i.e. DSO/DLL) as well as debugging and logging functionality.

<ul>
  <li>
  On the Windows platform the library is called <b><i>util.lib</i></b>.
  </li><li>
  On the Linux platform the library is call <b><i>libmlutil.a</i></b>.
  </li>
</ul>

These libraries are also available as shared libraries.

<ul>
  <li>
  On the Windows platform the library is called <b><i>util.dll</i></b>.
  </li><li>
  On the Linux platform the library is call <b><i>libmlutil.so</i></b>.
  </li>
</ul>

## Build Instructions

### Build Instructions for Windows Platform

The build instructions for <i>util.lib</i> may be found on the
[mle_documentation project wiki](https://github.com/magic-lantern-studio/mle-documentation/wiki/SDK-Build-Environment-using-Microsoft-Visual-Studio#build-the-core-util-library-for-tools).

### Build Instructions for Linux Platform

The build instructions for <i>libmlutil.a</i> may be found on the
[mle-documentation project wiki](https://github.com/magic-lantern-studio/mle-documentation/wiki/SDK-Build-Environment-for-Linux-Platform#build-the-core-util-library-for-tools).
