const nativeGetChildren = require("bindings")("addon");

function getChildren({folderPath}){
    return nativeGetChildren(folderPath);
}

module.exports = getChildren;

