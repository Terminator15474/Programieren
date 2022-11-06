const express = require("express");
const fs = require('fs');

const app = express();

app.get('/', (req, res) => {
    res.sendFile('A:\\Programieren\\Video_website\\site.html', (error) => {if(error) console.log(error?.message)})
});

app.get('/files/:filename', (req, res) => {
    if(req.params.filename == 'favicon.ico') return;
    console.log(req.params.filename);
    res.sendFile("A:\\Programieren\\Video_website\\" + req.params.filename);
});

app.get('/get-vid/:filename', (req, res) => {
        res.sendFile("A:\\Docs\\" + req.params.filename);
});

app.get('/video-filenames', async (req, res) => {
    var dir = await fs.promises.opendir("A:\\Docs");
    var return_val = "";
    for await (var file of dir) {
        return_val += file.name + ";";
    }
    return_val[return_val.length-1] = "";
    res.send(return_val).status(200);
});

app.listen(80, () => {
    console.log("Listening on port 80");
});