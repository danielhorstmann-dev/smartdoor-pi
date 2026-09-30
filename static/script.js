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

statusLaden();

setInterval(
    statusLaden,
    1000
);
