const express = require('express');
const fs = require('fs');
const app = express();

app.get('/', (req, res) => {
    res.sendFile("./site.html");
});

app.get('/:file', (req, res) => {
    let file = req.params.file;
    console.log(file);
    fs.readFileSync("./" + file);
});

app.listen(8080, () => {console.log("Listening on port 8080");});