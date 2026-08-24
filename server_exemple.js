require("dotenv").config();

const express = require("express");
const mysql = require("mysql2");
const nodemailer = require("nodemailer");

const transporter = nodemailer.createTransport({
    service: "gmail",
    auth: {
        user: "gmail",
        pass: "gmail_password"
    }
});

const app = express();

const PORT = 3000;

const db = mysql.createConnection({
    host: process.env.DB_HOST,
    port: process.env.DB_PORT,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_NAME
});


function sendEmailAlert(avgSoil, alertId) {


    const mailOptions = {

        from: "Plant senseur",

        to: "mail@exemple.com",

        subject: "🌱 Alerte Plant Monitor",

        text:
        `
        Votre plante semble sèche.

        Valeur moyenne:
        ${avgSoil}
        `

    };


    transporter.sendMail(mailOptions, (error, info) => {


        if (error) {

            console.log("Erreur email:", error);

            return;

        }


        console.log("Email envoyé");
        markAlertSent(alertId);


    });

}

// =====================
// CHECK IF ALERT EXISTS TODAY
// =====================

function alertToday(sensor_id, callback) {

    const sql = `
        SELECT *
        FROM alerts
        WHERE sensor_id = ?
        AND alert_type = 'DRY_SOIL'
        AND sent = TRUE
        AND DATE(created_at) = CURDATE()
    `;


    db.query(sql, [sensor_id], (err, results) => {

        if (err) {
            console.log(err);
            callback(false);
            return;
        }


        callback(results.length > 0);

    });

}

function markAlertSent(alertId) {


    const sql = `
        UPDATE alerts
        SET sent = TRUE,
            sent_at = NOW()
        WHERE id = ?
    `;


    db.query(sql, [alertId], (err)=>{

        if(err){
            console.log(err);
            return;
        }

        console.log("Alerte marquée envoyée");

    });

}

// =====================
// SAVE ALERT
// =====================

function saveAlert(sensor_id, avgSoil, callback) {


    const sql = `
        INSERT INTO alerts
        (sensor_id, alert_type, message, value)
        VALUES (?, ?, ?, ?)
    `;


    const values = [
        sensor_id,
        "DRY_SOIL",
        "La plante semble sèche",
        avgSoil
    ];


    db.query(sql, values, (err, result) => {


        if (err) {
            console.log(err);
            return;
        }


        console.log("Alerte créée");

        callback(result.insertId);

    });

}

function checkPlant() {


    const sql = `
        SELECT AVG(soil) AS avg_soil
        FROM (
            SELECT soil
            FROM sensor_data
            WHERE sensor_id = 1
            ORDER BY timestamp DESC
            LIMIT 100
        ) AS recent
    `;


    db.query(sql, (err, results)=>{


        const avgSoil = results[0].avg_soil;


        console.log("Moyenne sol:", avgSoil);



        if(avgSoil < 30){


            console.log("Sol sec détecté 🌵");



            alertToday(1, (alreadySent)=>{


                if(alreadySent){


                    console.log("Alerte déjà envoyée aujourd'hui");


                } else {


                    console.log("Nouvelle alerte");


                    saveAlert(1, avgSoil, (alertId)=>{


                        sendEmailAlert(avgSoil, alertId);


                    });


                }


            });



        } else {


            console.log("🌱 Tout va bien");


        }


    });


}

// Permet de recevoir du JSON
app.use(express.json());


// Route de test
app.get("/", (req, res) => {
    res.send("Plant Monitor API fonctionne 🌱");
});


// Route pour recevoir les données du capteur
app.post("/sensor", (req, res) => {

    const data = req.body;

    console.log("Nouvelle mesure reçue:");
    console.log(data);

    const sql = `
        INSERT INTO sensor_data
        (sensor_id, soil, temperature, humidity)
        VALUES (?, ?, ?, ?)
    `;

    db.query(
        sql,
        [
            data.sensor_id,
            data.soil,
            data.temperature,
            data.humidity
        ],
        (err, result) => {

            if (err) {
                console.error("Erreur SQL :", err);

                return res.status(500).json({
                    status: "error",
                    message: "Erreur insertion MySQL"
                });
            }

            console.log("Mesure enregistrée avec ID :", result.insertId);

            res.json({
                status: "success",
                message: "Données enregistrées",
                id: result.insertId
            });

        }
    );

});


// Démarrer le serveur
app.listen(PORT, "0.0.0.0", () => {
    console.log(`Serveur actif sur le port ${PORT}`);
});

// Analyse automatique toutes les 30 secondes
setInterval(checkPlant, 30000);