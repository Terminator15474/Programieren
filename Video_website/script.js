httpGetAsync('/video-filenames', (res) => {
    var videos = res.split(';');
    videos[videos.length-1] = null;
    var counter = 0;
    var videos_div = document.getElementById('videos');
    for (var video of videos) {
        if(video == null) continue;
        videos_div.innerHTML += '<video autostart=1 src="./get-vid/' + video + '" class="video-element" width="910" height="512" controls="true"></video>'
    }
    for (var video_elem of videos_div.children) {
        video_elem.pause();
    }
});

function httpGetAsync(theUrl, callback)
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            callback(xmlHttp.responseText);
    }
    xmlHttp.open("GET", theUrl, true);
    xmlHttp.send(null);
}