
// Bindings utilities

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts big (string or array) values into a C-style storage, in temporary space

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
var ensureCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare: function() {
    if (ensureCache.needed) {
      // clear the temps
      for (var i = 0; i < ensureCache.temps.length; i++) {
        Module['_free'](ensureCache.temps[i]);
      }
      ensureCache.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_free'](ensureCache.buffer);
      ensureCache.buffer = 0;
      ensureCache.size += ensureCache.needed;
      // clean up
      ensureCache.needed = 0;
    }
    if (!ensureCache.buffer) { // happens first time, or when we need to grow
      ensureCache.size += 128; // heuristic, avoid many small grow events
      ensureCache.buffer = Module['_malloc'](ensureCache.size);
      assert(ensureCache.buffer);
    }
    ensureCache.pos = 0;
  },
  alloc: function(array, view) {
    assert(ensureCache.buffer);
    var bytes = view.BYTES_PER_ELEMENT;
    var len = array.length * bytes;
    len = (len + 7) & -8; // keep things aligned to 8 byte boundaries
    var ret;
    if (ensureCache.pos + len >= ensureCache.size) {
      // we failed to allocate in the buffer, ensureCache time around :(
      assert(len > 0); // null terminator, at least
      ensureCache.needed += len;
      ret = Module['_malloc'](len);
      ensureCache.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = ensureCache.buffer + ensureCache.pos;
      ensureCache.pos += len;
    }
    return ret;
  },
  copy: function(array, view, offset) {
    offset >>>= 0;
    var bytes = view.BYTES_PER_ELEMENT;
    switch (bytes) {
      case 2: offset >>>= 1; break;
      case 4: offset >>>= 2; break;
      case 8: offset >>>= 3; break;
    }
    for (var i = 0; i < array.length; i++) {
      view[offset + i] = array[i];
    }
  },
};

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureString(value) {
  if (typeof value === 'string') {
    var intArray = intArrayFromString(value);
    var offset = ensureCache.alloc(intArray, HEAP8);
    ensureCache.copy(intArray, HEAP8, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt8(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP8);
    ensureCache.copy(value, HEAP8, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt16(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP16);
    ensureCache.copy(value, HEAP16, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP32);
    ensureCache.copy(value, HEAP32, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF32);
    ensureCache.copy(value, HEAPF32, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat64(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF64);
    ensureCache.copy(value, HEAPF64, offset);
    return offset;
  }
  return value;
}


// VoidPtr
/** @suppress {undefinedVars, duplicate} @this{Object} */function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

  VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};
// ImageOperations
/** @suppress {undefinedVars, duplicate} @this{Object} */function ImageOperations() {
  this.ptr = _emscripten_bind_ImageOperations_ImageOperations_0();
  getCache(ImageOperations)[this.ptr] = this;
};;
ImageOperations.prototype = Object.create(WrapperObject.prototype);
ImageOperations.prototype.constructor = ImageOperations;
ImageOperations.prototype.__class__ = ImageOperations;
ImageOperations.__cache__ = {};
Module['ImageOperations'] = ImageOperations;

ImageOperations.prototype['createImages'] = ImageOperations.prototype.createImages = /** @suppress {undefinedVars, duplicate} @this{Object} */function(base46Str) {
  var self = this.ptr;
  ensureCache.prepare();
  if (base46Str && typeof base46Str === 'object') base46Str = base46Str.ptr;
  else base46Str = ensureString(base46Str);
  _emscripten_bind_ImageOperations_createImages_1(self, base46Str);
};;

ImageOperations.prototype['grayscale_avgs'] = ImageOperations.prototype.grayscale_avgs = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_ImageOperations_grayscale_avgs_0(self);
};;

ImageOperations.prototype['grayscale_lums'] = ImageOperations.prototype.grayscale_lums = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_ImageOperations_grayscale_lums_0(self);
};;

ImageOperations.prototype['colorMasks'] = ImageOperations.prototype.colorMasks = /** @suppress {undefinedVars, duplicate} @this{Object} */function(r, g, b) {
  var self = this.ptr;
  if (r && typeof r === 'object') r = r.ptr;
  if (g && typeof g === 'object') g = g.ptr;
  if (b && typeof b === 'object') b = b.ptr;
  _emscripten_bind_ImageOperations_colorMasks_3(self, r, g, b);
};;

ImageOperations.prototype['encodeMessages'] = ImageOperations.prototype.encodeMessages = /** @suppress {undefinedVars, duplicate} @this{Object} */function(message) {
  var self = this.ptr;
  ensureCache.prepare();
  if (message && typeof message === 'object') message = message.ptr;
  else message = ensureString(message);
  _emscripten_bind_ImageOperations_encodeMessages_1(self, message);
};;

ImageOperations.prototype['decodeMessagesLib'] = ImageOperations.prototype.decodeMessagesLib = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  return UTF8ToString(_emscripten_bind_ImageOperations_decodeMessagesLib_0(self));
};;

ImageOperations.prototype['encodeBytes'] = ImageOperations.prototype.encodeBytes = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  return UTF8ToString(_emscripten_bind_ImageOperations_encodeBytes_0(self));
};;

ImageOperations.prototype['getMetrics'] = ImageOperations.prototype.getMetrics = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  return UTF8ToString(_emscripten_bind_ImageOperations_getMetrics_0(self));
};;

ImageOperations.prototype['flipX'] = ImageOperations.prototype.flipX = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_ImageOperations_flipX_0(self);
};;

ImageOperations.prototype['flipY'] = ImageOperations.prototype.flipY = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_ImageOperations_flipY_0(self);
};;

ImageOperations.prototype['std_convolve_clamp_to_0'] = ImageOperations.prototype.std_convolve_clamp_to_0 = /** @suppress {undefinedVars, duplicate} @this{Object} */function(channel, ker_w, ker_h, cr, cc) {
  var self = this.ptr;
  if (channel && typeof channel === 'object') channel = channel.ptr;
  if (ker_w && typeof ker_w === 'object') ker_w = ker_w.ptr;
  if (ker_h && typeof ker_h === 'object') ker_h = ker_h.ptr;
  if (cr && typeof cr === 'object') cr = cr.ptr;
  if (cc && typeof cc === 'object') cc = cc.ptr;
  _emscripten_bind_ImageOperations_std_convolve_clamp_to_0_5(self, channel, ker_w, ker_h, cr, cc);
};;

ImageOperations.prototype['std_convolve_clamp_to_border'] = ImageOperations.prototype.std_convolve_clamp_to_border = /** @suppress {undefinedVars, duplicate} @this{Object} */function(channel, ker_w, ker_h, cr, cc) {
  var self = this.ptr;
  if (channel && typeof channel === 'object') channel = channel.ptr;
  if (ker_w && typeof ker_w === 'object') ker_w = ker_w.ptr;
  if (ker_h && typeof ker_h === 'object') ker_h = ker_h.ptr;
  if (cr && typeof cr === 'object') cr = cr.ptr;
  if (cc && typeof cc === 'object') cc = cc.ptr;
  _emscripten_bind_ImageOperations_std_convolve_clamp_to_border_5(self, channel, ker_w, ker_h, cr, cc);
};;

ImageOperations.prototype['Undos'] = ImageOperations.prototype.Undos = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_ImageOperations_Undos_0(self);
};;

  ImageOperations.prototype['__destroy__'] = ImageOperations.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_ImageOperations___destroy___0(self);
};