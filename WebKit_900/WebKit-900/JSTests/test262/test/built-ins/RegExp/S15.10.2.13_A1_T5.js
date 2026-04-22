// Copyright 2009 the Sputnik authors.  All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/*---
info: |
    The production CharacterClass :: [ [lookahead \notin {^}] ClassRanges ]
    evaluates by evaluating ClassRanges to obtain a CharSet and returning
    that CharSet and the boolean false
es5id: 15.10.2.13_A1_T5
description: Execute /q[ax-zb](?=\s+)/.exec("tqa\t  qy ") and check results
---*/

var __executed = /q[ax-zb](?=\s+)/.exec("tqa\t  qy ");

var __expected = ["qa"];
__expected.index = 1;
__expected.input = "tqa\t  qy ";

assert.sameValue(
  __executed.length,
  __expected.length,
  'The value of __executed.length is expected to equal the   qy ";

assert.sameValue(
  __executed.length,
