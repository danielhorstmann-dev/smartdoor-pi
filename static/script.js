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

function pinAendern() {

    const daten = new URLSearchParams();

    daten.append(
        "altePin",
        document.getElementById(
            "altePin"
        ).value
    );

    daten.append(
        "neuePin",
        document.getElementById(
            "neuePin"
        ).value
    );

    daten.append(
        "pinBestaetigung",
        document.getElementById(
            "pinBestaetigung"
        ).value
    );

    fetch(
        "/api/pin/aendern",
        {
            method: "POST",
            body: daten
        }
    );

    document.getElementById("altePin").value = "";
    document.getElementById("neuePin").value = "";
    document.getElementById("pinBestaetigung").value = "";
}


function pinZuruecksetzen() {

    const daten = new URLSearchParams();

    daten.append(
        "neuePin",
        document.getElementById(
            "resetNeuePin"
        ).value
    );

    daten.append(
        "pinBestaetigung",
        document.getElementById(
            "resetBestaetigung"
        ).value
    );

    fetch(
        "/api/pin/reset",
        {
            method: "POST",
            body: daten
        }
    );

    document.getElementById("resetNeuePin").value = "";
    document.getElementById("resetBestaetigung").value = "";
}

statusLaden();

setInterval(
    statusLaden,
    1000
);
