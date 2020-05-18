Object.defineProperty(Uint8Array.prototype, 'slice', {
  value: function (begin, end)
   {
      return new Uint8Array(Array.prototype.slice.call(this, begin, end));
   }
});

Object.defineProperty(Uint8Array.prototype, 'toString', {
  value: function ()
   {
      return Array.prototype.join.call(this, ',');
   }
});

function importScripts(resource)
{
    switch (resource) {
        // These are bundled and available.
        case "//pako_inflate.js":
            return;
        default:
            throw("importScripts() Attempted on unknown resource " + resource);
    }
}
