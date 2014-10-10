function createmap(){
    return new Object();
}

function add(map, k, v) {
    map[k] = v;
}

function set(map, k, v) {
    map[k] = v;
}

function find(map, k) {
    return map[k];
}

function del(map, k) {
    var delval = map[k];
    delete map[k];
    for (var prop in map){
        if (delval < map[prop]){
            set(map, prop, map[prop]-1)
        }
    }
}

function display(map) {
    console.debug("js map display", map)
    for (var prop in map){
        console.debug(prop, "=", map[prop])
    }
}
