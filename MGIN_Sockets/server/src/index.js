class Eintrag {
    date;
    eintrag;
    constructor(date, eintrag) {
        this.date = date;
        this.eintrag = eintrag;
    }
}

class Patient {
    static counter = 0;    
    id;
    vorname;
    nachname;
    eintraege;
    constructor(vorname, nachname) {
        this.vorname = vorname;
        this.nachname = nachname;
        this.id = Patient.counter;
        Patient.counter++;
        this.eintraege = new Array();
    }

    addEintrag(e) {
        this.eintraege.push(e);
    }
}





const express = require("express");
const cors = require('cors');
const app = express();
app.use(cors())




let patientenListe = new Array();

patientenListe.push(new Patient("Fritz", "Mustermann"));
patientenListe.push(new Patient("Frieda", "Musterfrau"));


app.listen(8080, () => {
    console.log(`🚀 Server started on port 8080`)
});
/**
 * Abholen eines bestimmten Mitarbeiters
 */
app.get('/resources/patienten/patient/:id', (req, res) => {
    let c = req.params.id;
    let mSearch = (patientenListe.find(x => (x.id == c)));
    let temp = JSON.stringify(mSearch);
    console.log("Got connection on endpoint: Patienten");
    return res.send('' + temp);
});

/*
* Erstellen eines neuen Mitarbeiters, es wird die id des neuen Mitarbeiters an den Aufrufer geliefert
*/
app.get('/resources/patienten/newpatient/:vorname/:nachname', (req, res) => {
    let vorname = req.params.vorname;
    let nachname = req.params.nachname;
    let newMitarbeiter = new Patient(vorname, nachname);
    patientenListe.push(newMitarbeiter);
    console.log("Got connection on endpoint: New Patient");
    return res.send('{"id": ' + newMitarbeiter.id + '}');
});

app.get('/resources/patienten/patientenIDs', (req, res) => {
    let tempList = {};
    tempList['Liste der IDs'] = patientenListe.map(element => {
        let tempObject = {};
        tempObject['id'] = element.id;
        return tempObject;
    });
    let stringIDs = JSON.stringify(tempList);
    console.log("Got connection on endpoint: IDS");
    return res.send('' + stringIDs);
});

   

app.get('/resources/patienten/newentry/:id/:eintrag_value', (req, res) => {
    let tempPAtient = patientenListe.filter(x => x.id == req.params.id)[0];
    tempPAtient.addEintrag(new Eintrag(Date.now(), req.params.eintrag_value));
    let stringIDs = JSON.stringify(tempPAtient);
    console.log("Got connection on endpoint: New Entry");
    return res.send('' + stringIDs);
});
