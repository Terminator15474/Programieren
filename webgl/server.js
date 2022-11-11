const express = require('express');
const fs = require('fs');
const app = express();
const os = require('os');

let path = "";

if(os.hostname() == "LAPTOP-1ORE7K2H") {
    path = "C:/Users/jando";
} else {
    path = "A:";
}


app.get('/', (req, res) => {
    res.sendFile(path + "/Programieren/webgl/site.html");        
});

app.get('/:file', (req, res) => {
    let file = req.params.file;
    res.sendFile(path + "/Programieren/webgl/" + file);
});

app.get('/image/:path', (req, res) => {
    let l_path = req.params.path;
    res.sendFile(path + "/Programieren/html_canvas_effects/images/" + l_path);
});

app.get('/:directory/:subdir/:file', (req, res) => {
    let file = req.params.file;
    let directory = req.params.directory;
    let subdir = req.params.subdir;
    res.sendFile(path + "/Programieren/webgl/" + directory + "/" + subdir + "/"+ file);
});

app.listen(8080, () => {console.log("Listening on port 8080");});