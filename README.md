# WIN-GETCHILDREN-JS

Getting Folder's children with attributes: Node.js (libuv) function fs.readdir(path[, options], callback) returns parent folder's children names/paths without attributes.

Since native Windows Functions `FindFirstFileExW` and `FindNextFileW` return `WIN32_FIND_DATAW wfd` we developed a small C++ addon returning the children's paths and also attributes as JSON Array.

Usage is quite simple:
```javascript
import getChildren from "win-getchildren-js";
try {
    const children = getChildren({folderPath});
} catch(error) {
    //... You should catch errors though
}
```
