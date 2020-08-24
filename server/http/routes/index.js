var express = require('express');
var router = express.Router();

const sqlite3 = require('sqlite3').verbose();

/* GET home page. */
router.get('/', function (req, res, next) {
    res.render('index', { title: 'Moien' });
});

router.get('/moien', function (req, res, next) {
    // select infos from db
    let db = new sqlite3.Database('clis.db', sqlite3.OPEN_READWRITE | sqlite3.OPEN_CREATE, (err) => {
        if (err) {
            console.error(err.message);
        }
        // console.log('Connected to the clients database.');
    });

    let clis = {};
    db.serialize(() => {
        db.all(`SELECT cid, hostname, ip, os, utime FROM clients`, (err, rows) => {
            if (err) {
                return console.error(err.message);
            }
            rows.forEach((row) => {
                const cli = {
                    ip: row.ip,
                    hostname: row.hostname,
                    os: row.os,
                    utime: row.utime,
                };
                let jo = JSON.stringify(cli);
                console.log(jo);
                clis.push(cli);
            });
        });
    });
    console.log(clis);
    res.send(clis);
});

router.post('/moien', function (req, res, next) {
    console.log(req.body);
    res.json(req.body.os);

    let ip = req.ip;
    if (req.ip.startsWith("::ffff:")) { // ipv4
        ip = req.ip.substr(7);
    }

    // save infos into db
    let db = new sqlite3.Database('clis.db', sqlite3.OPEN_READWRITE | sqlite3.OPEN_CREATE, (err) => {
        if (err) {
            console.error(err.message);
        }
        // console.log('Connected to the clients database.');
    });

    db.serialize(() => {
        db.run(
            `CREATE TABLE IF NOT EXISTS clients (
                    cid INTEGER PRIMARY KEY,
                    hostname text,
                    ip text,
                    os text,
                    utime text)`
        ).get(`SELECT cid, ip, os, utime FROM clients WHERE ip=?`, [ip], (err, row) => {
            if (err) {
                return console.error(err.message);
            }
            if (row) {
                let cid = row.cid;
                let sql = `UPDATE clients SET hostname=?, ip=?, os=?, utime=datetime('now', 'localtime') where cid=?`;
                db.run(sql, [req.body.hostname, ip, req.body.os, cid], function (err) {
                    if (err) {
                        return console.log(err.message);
                    }
                    console.log(`Row(s) updated: ${this.changes}`);
                });
            } else {
                let sql = `INSERT INTO clients (hostname, ip, os, utime) VALUES(?, ?, ?, datetime('now', 'localtime'))`;
                db.run(sql, [req.body.hostname, ip, req.body.os], function (err) {
                    if (err) {
                        return console.log(err.message);
                    }
                    // get the last insert id
                    console.log(`A row has been inserted with rowid ${this.lastID}`);
                });
            }
            db.close((err) => {
                if (err) {
                    console.error(err.message);
                }
                // console.log('Close the database connection.');
            });
        });
    });
});

module.exports = router;
