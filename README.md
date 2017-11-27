This C++ project allows XMIT-format files to be restored to USS files without having
to use the RECEIVE TSO command. 

The details of XMIT files can be found in the [z/OS TSO/E Customization](https://www.ibm.com/support/knowledgecenter/SSLTBW_2.1.0/com.ibm.zos.v2r1.ikjb400/toc.htm) manual. The relevant portions are:

* [text units and text unit pointer lists](https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.ikjb400/tupls.htm)
* [Format of transmitted data](https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.ikjb400/tmdf.htm)

The tests for the code use the [Google C++ test framework](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md).