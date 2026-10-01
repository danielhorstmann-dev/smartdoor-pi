function statusLaden() {

    fetch("/api/status")
        .then(response => response.json())
        .then(data => {

            document.getElementById(
                "licht"
            ).innerText = data.licht;

            document.getElementById(
                "schloss"
            ).innerText = data.schloss;

            document.getElementById(
                "bewegung"
            ).innerText = data.bewegung;

            document.getElementById(
                "helligkeit"
            ).innerText = data.helligkeit;
        });
}

function lichtAn() {

    fetch(
        "/api/licht/an",
        {
            method: "POST"
        }
    );
}


function lichtAus() {

    fetch(
        "/api/licht/aus",
        {
            method: "POST"
        }
    );
}

function tuerEntriegeln() {
    fetch(
        "/api/tuer/entriegeln",
        {
            method: "POST"
        }
    );
}

function tuerVerriegeln() {
    fetch(
        "/api/tuer/verriegeln",
        {
            method: "POST"
        }
    );
}

function automatikAn() {
    fetch(
        "/api/automatik/an",
        {
            method: "POST"
        }
    );
}

function automatikAus() {
    fetch(
        "/api/automatik/aus",
        {
            method: "POST"
        }
    );
}

statusLaden();

setInterval(
    statusLaden,
    1000
);
