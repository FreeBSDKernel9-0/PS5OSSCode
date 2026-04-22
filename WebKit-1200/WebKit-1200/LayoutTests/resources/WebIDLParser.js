(function webpackUniversalModuleDefinition(root, factory) {
	if(typeof exports === 'object' && typeof module === 'object')
		module.exports = factory();
	else if(typeof define === 'function' && define.amd)
		define([], factory);
	else if(typeof exports === 'object')
		exports["WebIDL2"] = factory();
	else
		root["WebIDL2"] = factory();
})(globalThis, function() {
return /******/ (() => { // webpackBootstrap
/******/ 	"use strict";
/******/ 	var __webpack_modules__ = ([
/* 0 */,
/* 1 */
/***/ ((__unused_webpack___webpack_module__, __webpack_exports__, __webpack_require__) => {

__webpack_require__.r(__webpack_exports__);
/* harmony export */ __webpack_require__.d(__webpack_exports__, {
/* harmony export */   "parse": () => (/* binding */ parse)
/* harmony export */ });
/* harmony import */ var _tokeniser_js__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(2);
/* harmony import */ var _productions_enum_js__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(15);
/* harmony import */ var _productions_includes_js__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(16);
/* harmony import */ var _productions_extended_attributes_js__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(8);
/* harmony import */ var _productions_typedef_js__WEBPACK_IMPORTED_MODULE_4__ = __webpack_require__(17);
/* harmony import */ var _productions_callback_js__WEBPACK_IMPORTED_MODULE_5__ = __webpack_require__(18);
/* harmony import */ var _productions_interface_js__WEBPACK_IMPORTED_MODULE_6__ = __webpack_require__(19);
/* harmony import */ var _productions_mixin_js__WEBPACK_IMPORTED_MODULE_7__ = __webpack_require__(25);
/* harmony import */ var _productions_dictionary_js__WEBPACK_IMPORTED_MODULE_8__ = __webpack_require__(26);
/* harmony import */ var _productions_namespace_js__WEBPACK_IMPORTED_MODULE_9__ = __webpack_require__(28);
/* harmony import */ var _productions_callback_interface_js__WEBPACK_IMPORTED_MODULE_10__ = __webpack_require__(29);
/* harmony import */ var _productions_helpers_js__WEBPACK_IMPORTED_MODULE_11__ = __webpack_require__(4);













/**
 * @param {Tokeniser} tokeniser
 * @param {object} options
 * @param {boolean} [options.concrete]
 */
function parseByTokens(tokeniser, options) {
  const source = tokeniser.source;

  function error(str) {
    tokeniser.error(str);
  }

  function consume(...candidates) {
    return tokeniser.consume(...candidates);
  }

  function callback() {
    const callback = consume("callback");
    if (!callback) return;
    if (tokeniser.probe("interface")) {
      return _productions_callback_interface_js__WEBPACK_IMPORTED_MODULE_10__.CallbackInterface.parse(tokeniser, callback);
    }
    return _productions_callback_js__WEBPACK_IMPORTED_MODULE_5__.CallbackFunction.parse(tokeniser, callback);
  }

  function interface_(opts) {
    const base = consume("interface");
    if (!base) return;
    const ret =
      _productions_mixin_js__WEBPACK_IMPORTED_MODULE_7__.Mixin.parse(tokeniser, base, opts) ||
      _productions_interface_js__WEBPACK_IMPORTED_MODULE_6__.Interface.parse(tokeniser, base, opts) ||
      error("Interface has no proper body");
    return ret;
  }

  function partial() {
    const partial = consume("partial");
    if (!partial) return;
    return (
      _productions_dictionary_js__WEBPACK_IMPORTED_MODULE_8__.Dictionary.parse(tokeniser, { partial }) ||
      interface_({ partial }) ||
      _productions_namespace_js__WEBPACK_IMPORTED_MODULE_9__.Namespace.parse(tokeniser, { partial }) ||
      error("Partial doesn't apply to anything")
    );
  }

  function definition() {
    return (
      callback() ||
      interface_() ||
      partial() ||
      _productions_dictionary_js__WEBPACK_IMPORTED_MODULE_8__.Dictionary.parse(tokeniser) ||
      _productions_enum_js__WEBPACK_IMPORTED_MODULE_1__.Enum.parse(tokeniser) ||
      _productions_typedef_js__WEBPACK_IMPORTED_MODULE_4__.Typedef.parse(tokeniser) ||
      _productions_includes_js__WEBPACK_IMPORTED_MODULE_2__.Includes.parse(tokeniser) ||
      _productions_namespace_js__WEBPACK_IMPORTED_MODULE_9__.Namespace.parse(tokeniser)
    );
  }

  function definitions() {
    if (!source.length) return [];
    const defs = [];
    while (true) {
      const ea = _productions_extended_attributes_js__WEBPACK_IMPORTED_MODULE_3__.ExtendedAttributes.parse(tokeniser);
      const def = definition();
      if (!def) {
        if (ea.length) error("Stray extended attributes");
        break;
      }
      (0,_productions_helpers_js__WEBPACK_IMPORTED_MODULE_11__.autoParenter)(def).extAttrs = ea;
      defs.push(def);
    }
    const eof = tokeniser.consumeType("eof");
    if (options.concrete) {
      defs.push(eof);
    }
    return defs;
  }
  const res = definitions();
  if (tokeniser.position < source.length) error("Unrecognised tokens");
  return res;
}

/**
 * @param {string} str
 * @param {object} [options]
 * @param {*} [options.sourceName]
 * @param {boolean} [options.concrete]
 */
function parse(str, options = {}) {
  const tokeniser = new _tokeniser_js__WEBPACK_IMPORTED_MODULE_0__.Tokeniser(str);
  if (typeof options.sourceName !== "undefined") {
    tokeniser.source.name = options.sourceName;
  }
  return parseByTokens(tokeniser, options);
}


/***/ }),
/* 2 */
/***/ ((__unused_webpack___webpack_module__, __webpack_exports__, __webpack_require__) => {

__webpack_require__.r(__webpack_exports__);
/* harmony export */ __webpack_require__.d(__webpack_exports__, {
/* harmony export */   "typeNameKeywords": () => (/* binding */ typeNameKeywords),
/* harmony export */   "stringTypes": () => (/* binding */ stringTypes),
/* harmony export */   "argumentNameKeywords": () => (/* binding */ argumentNameKeywords),
/* harmony export */   "Tokeniser": () => (/* binding */ Tokeniser),
/* harmony export */   "WebIDLParseError": () => (/* binding */ WebIDLParseError)
/* harmony export */ });
/* harmony import */ var _error_js__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(3);
/* harmony import */ var _productions_helpers_js__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(4);



// These regular expressions use the sticky flag so they will only match at
// the current location (ie. the offset of lastIndex).
const tokenRe = {
  // This expression uses a lookahead assertion to catch false matches
  // against integers early.
  decimal:
    /-?(?=[0-9]*\.|[0-9]+[eE])(([0-9]+\.[0-9]*|[0-9]*\.[0-9]+)([Ee][-+]?[0-9]+)?|[0-9]+[Ee][-+]?[0-9]+)/y,
  integer: /-?(0([Xx][0-9A-Fa-f]+|[0-7]*)|[1-9][0-9]*)/y,
  identifier: /[_-]?[A-Za-z][0-9A-Z_a-z-]*/y,
  string: /"[^"]*"/y,
  whitespace: /[\t\n\r ]+/y,
  comment: /\/\/.*|\/\*(.|\n)*?\*\//y,
  other: /[^\t\n\r 0-9A-Za-z]/y,
};

const typeNameKeywords = [
  "ArrayBuffer",
  "DataView",
  "Int8Array",
  "Int16Array",
  "Int32Array",
  "Uint8Array",
  "Uint16Array",
  "Uint32Array",
  "Uint8ClampedArray",
  "Float32Array",
  "Float64Array",
  "any",
  "object",
  "symbol",
];

const stringTypes = ["ByteString", "DOMString", "USVString"];

const argumentNameKeywords = [
  "async",
  "attribute",
  "callback",
  "const",
  "constructor",
  "deleter",
  "dictionary",
  "enum",
  "getter",
  "includes",
  "inherit",
  "interface",
  "iterable",
  "maplike",
  "namespace",
  "partial",
  "required",
  "setlike",
  "setter",
  "static",
  "stringifier",
  "typedef",
  "unrestricted",
];

const nonRegexTerminals = [
  "-Infinity",
  "FrozenArray",
  "Infinity",
  "NaN",
  "ObservableArray",
  "Promise",
  "bigint",
  "boolean",
  "byte",
  "double",
  "false",
  "float",
  "long",
  "mixin",
  "null",
  "octet",
  "optional",
  "or",
  "readonly",
  "record",
  "sequence",
  "short",
  "true",
  "undefined",
  "unsigned",
  "void",
].concat(argumentNameKeywords, stringTypes, typeNameKeywords);

const punctuations = [
  "(",
  ")",
  ",",
  "...",
  ":",
  ";",
  "<",
  "=",
  ">",
  "?",
  "[",
  "]",
  "{",
  "}",
];

const reserved = [
  // "constructor" is now a keyword
  "_constructor",
  "toString",
  "_toString",
];

/**
 * @typedef {ArrayItemType<ReturnType<typeof tokenise>>} Token
 * @param {string} str
 */
function tokenise(str) {
  const tokens = [];
  let lastCharIndex = 0;
  let trivia = "";
  let line = 1;
  let index = 0;
  while (lastCharIndex < str.length) {
    const nextChar = str.charAt(lastCharIndex);
    let result = -1;

    if (/[\t\n\r ]/.test(nextChar)) {
      result = attemptTokenMatch("whitespace", { noFlushTrivia: true });
    } else if (nextChar === "/") {
      result = attemptTokenMatch("comment", { noFlushTrivia: true });
    }

    if (result !== -1) {
      const currentTrivia = tokens.pop().value;
      line += (currentTrivia.match(/\n/g) || []).length;
      trivia += currentTrivia;
      index -= 1;
    } else if (/[-0-9.A-Z_a-z]/.test(nextChar)) {
      result = attemptTokenMatch("decimal");
      if (result === -1) {
        result = attemptTokenMatch("integer");
      }
      if (result === -1) {
        result = attemptTokenMatch("identifier");
        const lastIndex = tokens.length - 1;
        const token = tokens[lastIndex];
        if (result !== -1) {
          if (reserved.includes(token.value)) {
            const message = `${(0,_productions_helpers_js__WEBPACK_IMPORTED_MODULE_1__.unescape)(
              token.value
            )} is a reserved identifier and must not be used.`;
            throw new WebIDLParseError(
              (0,_error_js__WEBPACK_IMPORTED_MODULE_0__.syntaxError)(tokens, lastIndex, null, message)
            );
          } else if (nonRegexTerminals.includes(token.value)) {
            token.type = "inline";
          }
        }
      }
    } else if (nextChar === '"') {
      result = attemptTokenMatch("string");
    }

    for (const punctuation of punctuations) {
      if (str.startsWith(punctuation, lastCharIndex)) {
        tokens.push({
          type: "inline",
          value: punctuation,
          trivia,
          line,
          index,
        });
        trivia = "";
        lastCharIndex += punctuation.length;
        result = lastCharIndex;
        break;
      }
    }

    // other as the last try
    if (result === -1) {
      result = attemptTokenMatch("other");
    }
    if (result === -1) {
      throw new Error("Token stream not progressing");
    }
    lastCharIndex = result;
    index += 1;
  }

  // remaining trivia as eof
  tokens.push({
    type: "eof",
    value: "",
    trivia,
  });

  return tokens;

  /**
   * @param {keyof typeof tokenRe} type
   * @param {object} options
   * @param {boolean} [options.noFlushTrivia]
   */
  function attemptTokenMatch(type, { noFlushTrivia } = {}) {
    const re = tokenRe[type];
    re.lastIndex = lastCharIndex;
    const result = re.exec(str);
    if (result) {
      tokens.push({ type, value: result[0], trivia, line, index });
      if (!noFlushTrivia) {
        trivia = "";
      }
      return re.lastIndex;
    }
    return -1;
  }
}

class Tokeniser {
  /**
   * @param {string} idl
   */
  constructor(idl) {
    this.source = tokenise(idl);
    this.position = 0;
  }

  /**
   * @param {string} message
   * @return {never}
   */
  error(message) {
    throw new WebIDLParseError(
      (0,_error_js__WEBPACK_IMPORTED_MODULE_0__.syntaxError)(this.source, this.position, this.current, message)
    );
  }

  /**
   * @param {string} type
   */
  probeType(type) {
    return (
      this.source.length > this.position &&
      this.source[this.position].type =  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r  r