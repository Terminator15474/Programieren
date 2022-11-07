const express = require('express');
const fs = require('fs');
const app = express();

app.get('/', (req, res) => {
    res.sendFile("A:/Programieren/webgl/site.html");
});

app.get('/:file', (req, res) => {
    let file = req.params.file;
    res.sendFile("A:/Programieren/webgl/" + file);
});

app.get('/:directory/:subdir/:file', (req, res) => {
    let file = req.params.file;
    let directory = req.params.directory;
    let subdir = req.params.subdir;
    console.log(directory + "/" + subdir + "/" + file);
    res.sendFile("A:/Programieren/webgl/" + directory + "/" + subdir + "/"+ file);
});

app.listen(8080, () => {console.log("Listening on port 8080");});